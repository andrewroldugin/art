/**
 * Copyright (c) 2012-2013, Stephen Blackheath and Anthony Jones
 * Released under a BSD3 licence.
 *
 * C++ implementation courtesy of International Telematics Ltd.
 */
#ifndef _SODIUM_TRANSACTION_H_
#define _SODIUM_TRANSACTION_H_

#include <sodium/count_set.h>
#include <sodium/light_ptr.h>
#include <sodium/lock_pool.h>
#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>
#include <boost/intrusive_ptr.hpp>
#include <sodium/unit.h>
#include <pthread.h>
#include <map>
#include <set>
#include <list>
#include <forward_list>
#include <limits.h>

namespace sodium {

    class mutex
    {
    private:
        pthread_mutex_t mx;
        // ensure we don't copy or assign a mutex by value
        mutex(const mutex&) {}
        mutex& operator = (const mutex&) { return *this; }
    public:
        mutex();
        ~mutex();
        void lock()
        {
            pthread_mutex_lock(&mx);
        }
        void unlock()
        {
            pthread_mutex_unlock(&mx);
        }
    };

    struct partition {
        partition();
        ~partition();
        mutex mx;
        int depth;
        pthread_key_t key;
        bool processing_post;
        std::list<std::function<void()>> postQ;
        void post(const std::function<void()>& action);
        void process_post();
    };

    /*!
     * The default partition which gets chosen when you don't specify one.
     */
    struct def_part {
        static partition* part();
    };

    namespace impl {
        struct transaction_impl;

        typedef unsigned long rank_t;
        #define SODIUM_IMPL_RANK_T_MAX ULONG_MAX

        class node;
        template <class Allocator>
        struct listen_impl_func {
            typedef std::function<std::function<void()>*(
                transaction_impl*,
                const std::shared_ptr<impl::node>&,
                std::function<void(const std::shared_ptr<impl::node>&, transaction_impl*, const light_ptr&)>*,
                bool)> closure;
            listen_impl_func(closure* func)
                : func(func) {}
            ~listen_impl_func()
            {
                assert(cleanups.begin() == cleanups.end() && func == NULL);
            }
            count_set counts;
            closure* func;
            std::forward_list<std::function<void()>*> cleanups;
            inline void update_and_unlock(spin_lock* l) {
                if (func && !counts.active()) {
                    counts.inc_strong();
                    l->unlock();
                    for (auto it = cleanups.begin(); it != cleanups.end(); ++it) {
                        (**it)();
                        delete *it;
                    }
                    cleanups.clear();
                    delete func;
                    func = NULL;
                    l->lock();
                    counts.dec_strong();
                }
                if (!counts.alive()) {
                    l->unlock();
                    delete this;
                }
                else
                    l->unlock();
            }
        };

        struct H_EVENT {};
        struct H_STRONG {};
        struct H_NODE {};

        void intrusive_ptr_add_ref(sodium::impl::listen_impl_func<sodium::impl::H_EVENT>* p);
        void intrusive_ptr_release(sodium::impl::listen_impl_func<sodium::impl::H_EVENT>* p);
        void intrusive_ptr_add_ref(sodium::impl::listen_impl_func<sodium::impl::H_STRONG>* p);
        void intrusive_ptr_release(sodium::impl::listen_impl_func<sodium::impl::H_STRONG>* p);
        void intrusive_ptr_add_ref(sodium::impl::listen_impl_func<sodium::impl::H_NODE>* p);
        void intrusive_ptr_release(sodium::impl::listen_impl_func<sodium::impl::H_NODE>* p);

        inline bool alive(const boost::intrusive_ptr<listen_impl_func<H_STRONG>>& li) {
            return li && li->func != NULL;
        }

        inline bool alive(const boost::intrusive_ptr<listen_impl_func<H_EVENT>>& li) {
            return li && li->func != NULL;
        }

        class node
        {
            public:
                struct target {
                    target(
                        void* h,
                        const std::shared_ptr<node>& n
                    ) : h(h),
                        n(n) {}

                    void* h;
                    std::shared_ptr<node> n;
                };

            public:
                node();
                node(rank_t rank);
                ~node();

                rank_t rank;
                std::forward_list<node::target> targets;
                std::forward_list<light_ptr> firings;
                std::forward_list<boost::intrusive_ptr<listen_impl_func<H_EVENT>>> sources;
                boost::intrusive_ptr<listen_impl_func<H_NODE>> listen_impl;

                void link(void* holder, const std::shared_ptr<node>& target);
                void unlink(void* holder);

            private:
                void ensure_bigger_than(std::set<node*>& visited, rank_t limit);
        };
    }
}

namespace sodium {
    namespace impl {

        template <class A>
        struct ordered_value {
            ordered_value() : tid(-1) {}
            long long tid;
            boost::optional<A> oa;
        };

        struct entryID {
            entryID() : id(0) {}
            entryID(rank_t id) : id(id) {}
            rank_t id;
            entryID succ() const { return entryID(id+1); }
            inline bool operator < (const entryID& other) const { return id < other.id; }
        };

        rank_t rankOf(const std::shared_ptr<node>& target);

        struct prioritized_entry {
            prioritized_entry(const std::shared_ptr<node>& target,
                              const std::function<void(transaction_impl*)>& action)
                : target(target), action(action)
            {
            }
            std::shared_ptr<node> target;
            std::function<void(transaction_impl*)> action;
        };

        struct transaction_impl {
            transaction_impl(partition* part);
            ~transaction_impl();
            partition* part;
            entryID next_entry_id;
            std::map<entryID, prioritized_entry> entries;
            std::multimap<rank_t, entryID> prioritizedQ;
            std::list<std::function<void()>> lastQ;

            void prioritized(const std::shared_ptr<impl::node>& target,
                             const std::function<void(impl::transaction_impl*)>& action);
            void last(const std::function<void()>& action);

            bool to_regen;
            void check_regen();
            void process_transactional();
        };
    };

    class policy {
    public:
        policy() {}
        virtual ~policy() {}

        static policy* get_global();
        static void set_global(policy* policy);

        /*!
         * Get the current thread's active transaction for this partition, or NULL
         * if none is active.
         */
        virtual impl::transaction_impl* current_transaction(partition* part) = 0;

        virtual void initiate(impl::transaction_impl* impl) = 0;

        /*!
         * Dispatch the processing for this transaction according to the policy.
         * Note that post() will delete impl, so don't reference it after that.
         */
        virtual void dispatch(impl::transaction_impl* impl,
            const std::function<void()>& transactional,
            const std::function<void()>& post) = 0;
    };

    namespace impl {
        class transaction_ {
        private:
            transaction_impl* impl_;
            transaction_(const transaction_&) {}
            transaction_& operator = (const transaction_&) { return *this; };
        public:
            transaction_(partition* part);
            ~transaction_();
            impl::transaction_impl* impl() const { return impl_; }
        };
    };

    template <class P = def_part>
    class transaction : public impl::transaction_
    {
        private:
            transaction(const transaction<P>& other) {}
            transaction<P>& operator = (const transaction<P>& other) { return *this; };
        public:
            transaction() : transaction_(P::part()) {}
    };

    class simple_policy : public policy
    {
    public:
        simple_policy();
        virtual ~simple_policy();
        virtual impl::transaction_impl* current_transaction(partition* part);
        virtual void initiate(impl::transaction_impl* impl);
        virtual void dispatch(impl::transaction_impl* impl,
            const std::function<void()>& transactional,
            const std::function<void()>& post);
    };
};  // end namespace sodium

#endif


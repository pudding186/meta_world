#pragma once
#include "list_value_get.hpp"
#include <set>
#include <functional>
#include <queue>

namespace FSM
{
    class Event
    {
    public:
    protected:
    private:
    };

    template<typename T>
    class TEvent : public Event
    {
    public:
        static const char* Name(void)
        {
            static const char* name = typeid(T).name();

            return name;
        }

        static size_t Type(void)
        {
            static size_t hash_code = typeid(T).hash_code();

            return hash_code;
        }
    protected:
    private:
    };

    template<typename... E>
    struct EventList {};

    template<>
    struct EventList<>{};

    template <typename First, typename... Rest>
    struct EventList<First, Rest...>
        :public EventList<Rest...>
    {

        const static size_t SizeOf = (sizeof...(Rest) + 1);

        typename std::enable_if<
            std::is_base_of<Event, First>::value, First>::type value;

        void ToArray(Event* (&array)[SizeOf])
        {
            array[0] = &value;
            int init[] = {
                (array[ListGetIndex<Rest, SEventList<Rest...>>::index] = &ListGetValueByType<Rest>(*this), 0)...
            };
        }

        template<typename E>
        static size_t IndexOfEvent()
        {
            return indexOf<E, 0, First, Rest...>::value;
        }
    };

    class State
    {
    public:
        virtual void OnEvent(Event* event) = 0;
    protected:
    private:
    };

    template<typename T>
    class TState : public State
    {
    public:
        static const char* Name(void)
        {
            static const char* name = typeid(T).name();

            return name;
        }

        static size_t Type(void)
        {
            static size_t hash_code = typeid(T).hash_code();

            return hash_code;
        }

        

    protected:
    private:
    };

    template<typename... S>
    struct StateList{};

    template<>
    struct StateList<>{};

    template <typename First, typename... Rest>
    struct StateList<First, Rest...>
        :public StateList<Rest...>
    {
        const static size_t SizeOf = (sizeof...(Rest) + 1);
        typename std::enable_if<
            std::is_base_of<State, First>::value, First>::type value;

        void ToArray(State* (&array)[SizeOf])
        {
            array[0] = &value;
            int init[] = {
                (array[ListGetIndex<Rest, EventList<Rest...>>::index] = &ListGetValueByType<Rest>(*this), 0)...
            };
        }

        template<typename S>
        static size_t IndexOfState()
        {
            return indexOf<S, 0, First, Rest...>::value;
        }
    };

    struct StateTransInfo
    {
        size_t current_state;
        size_t event;
        size_t next_state;

        bool operator < (const StateTransInfo& info) const 
        {
            if (current_state < info.current_state)
            {
                return true;
            }
            else if (current_state == info.current_state)
            {
                return event < info.event;
            }

            return false;
        }
    };

    template<typename SL, typename EL>
    class StateMachine
    {
    public:
        StateMachine()
        {
            m_state_list.ToArray(m_state_array);
        }

        static void InitStateTransTable()
        {
            for (size_t i = 0; i < SL::SizeOf; i++)
            {
                for (size_t j = 0; j < EL::SizeOf; j++)
                {
                    GetStateTransTable()[i][j] = SL::SizeOf;
                }
            }
        }

        template<typename SC, typename EVT, typename SN>
        static void SetTransition()
        {
            GetStateTransTable()[SL:: template IndexOfState<SC>()][EL::template IndexOfEvent<EVT>()] = SL::template IndexOfState<SN>();
        }

        template<typename EVT, typename...Args>
        void Handle(Args&&...args)
        {
            bool empty = m_event_list.empty();
            EventData data;
            data.event = S_NEW(EVT, 1, std::forward<Args>(args)...);
            data.index = EL::template IndexOfEvent<EVT>();
            m_event_list.push(data);
            if (empty)
            {
                while (!m_event_list.empty())
                {
                    EventData& data = m_event_list.front();
                    _TransState(data);
                    m_event_list.pop();
                    S_DELETE(data.event);
                }
            }
        }

        template<typename EVT, typename...Args>
        void Trans(Args&&...args)
        {

        }

        inline static size_t (&GetStateTransTable())[SL::SizeOf][EL::SizeOf]
        {
            static size_t s_state_trans_table[SL::SizeOf][EL::SizeOf];
            return s_state_trans_table;
        }

    protected:

        using EventData = struct stEventData 
        {
            Event* event;
            size_t index;
        };

        void _TransState(const EventData& data)
        {
            size_t next_state_index = GetStateTransTable()[m_cur_state_index][data.index];

            if (next_state_index != SL::SizeOf)
            {
                m_last_state_index = m_cur_state_index;
                m_cur_state_index = next_state_index;

                m_state_array[m_last_state_index]->OnEvent(data.event);
            }
        }

    private:
        SL m_state_list;
        std::queue<EventData> m_event_list;
        State* m_state_array[SL::SizeOf];
        size_t m_cur_state_index;
        size_t m_last_state_index;
    };
}

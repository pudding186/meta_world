#pragma once

#include "smemory.hpp"
#include "guid_system.hpp"
#include "timer_system.hpp"
#include <map>
#include <vector>

template<typename K>
class ObjectSystem;

template<typename K>
class Object
{
    friend class ObjectSystem<K>;
public:
    Object(const K& key):m_key(key), m_ref(0)
    {
    }

    virtual ~Object()
    {
    }

    inline const K& Key() const 
    {
        return m_key;
    }

    inline size_t Ref() const 
    {
        return m_ref;
    }

    inline bool IsValid() const
    {
        return m_ref != 0;
    }

protected:
    void inline _SetRef(size_t ref) { m_ref = ref; }
private:
    K   m_key;
    size_t m_ref;
};

template<typename K, typename T>
class TObject : public Object<K>
{
public:
    TObject(const K& key):Object<K>(key)
    {

    }

protected:
private:
};

template<typename K>
class ObjectSystem
{
public:
    using ObjectRef = union uObjectRef
    {
        Object<K>* obj_ptr;
        size_t  ref;
    };
public:
    void Init(size_t initial_size)
    {
        m_object_map.clear();
        m_object_table.resize(initial_size);

        for (size_t i = 0; i < initial_size; i++)
        {
            m_object_table[i].ref = i + 1;
        }

        m_object_table[initial_size - 1].ref = 0;
    }
public:
    template<typename T, typename... Args>
    T* Create(const K& key, Args&&... args)
    {
        auto ret = m_object_map.insert(std::make_pair(key, nullptr));

        if (ret.second)
        {
            size_t free_slot = m_object_table[0].ref;
            if (!free_slot)//no free slot
            {
                size_t old_size = m_object_table.size();
                m_object_table.resize(old_size + old_size / 2);

                for (size_t i = old_size; i < m_object_table.size(); i++)
                {
                    m_object_table[i].ref = i + 1;
                }
                m_object_table[0].ref = old_size;
                m_object_table[m_object_table.size() - 1].ref = 0;

                free_slot = old_size;
            }

            m_object_table[0].ref = m_object_table[free_slot].ref;
            m_object_table[free_slot].obj_ptr = S_NEW(T, 1, key, std::forward<Args>(args)...);
            m_object_table[free_slot].obj_ptr->_SetRef(free_slot);
            ret.first->second = m_object_table[free_slot].obj_ptr;

            return static_cast<T*>(m_object_table[free_slot].obj_ptr);
        }
        else
        {
            return nullptr;
        }
    }

    Object<K>* GetObject(const K& key, size_t ref = 0)
    {
        if (ref > 0 && ref < m_object_table.size())
        {
            const ObjectRef& obj_ref = m_object_table[ref];

            if (obj_ref.ref > m_object_table.size())
            {
                if (obj_ref.obj_ptr->Key() == key)
                {
                    return obj_ref.obj_ptr;
                }
            }
        }

        auto it = m_object_map.find(key);
        if (it == m_object_map.end())
        {
            return nullptr;
        }

        return it->second;
    }

    template<typename T>
    T* Get(const K& key, size_t ref = 0)
    {
        Object<K>* object = GetObject(key, ref);

        if (SMemory::IsValidClassMemory<T>(object))
        {
            return static_cast<T*>(object);
        }

        return nullptr;
    }

    void DestroyObject(Object<K>* object)
    {
        if (object && object->Ref())
        {
            if (m_object_table[object->Ref()].obj_ptr != object)
            {
                throw "Oh Shit!";
            }

            m_object_table[object->Ref()].ref = m_object_table[0].ref;
            m_object_table[0].ref = object->Ref();
            object->_SetRef(0);

            sTimerSystem.AddDelayCaller(1, 1, this, &ObjectSystem<K>::OnDeleteObject, object);
        }
    }

    void Destroy(const K& key, size_t ref = 0)
    {
        DestroyObject(GetObject(key, ref));
    }

    void OnDeleteObject(Object<K>* object)
    {
        if (object->IsValid())
        {
            throw "Oh Shit!";
        }

        m_object_map.erase(object->Key());
        S_DELETE(object);
    }

protected:
private:
    std::map<K, Object<K>*> m_object_map;
    std::vector<typename ObjectSystem<K>::ObjectRef> m_object_table;
};
#pragma once
#include <cstdint>
#include <memory>

class VMT
{
public:
    VMT(void* instance)
        : m_instance(instance), m_vtable(nullptr), m_original_vtable(nullptr), m_vtable_size(0)
    {
        if (m_instance)
        {
            m_vtable = *static_cast<void***>(m_instance);
            m_original_vtable = m_vtable;
            while (m_original_vtable[m_vtable_size])
            {
                m_vtable_size++;
            }
            m_new_vtable = std::make_unique<void*[]>(m_vtable_size);
            std::memcpy(m_new_vtable.get(), m_original_vtable, m_vtable_size * sizeof(void*));
            *static_cast<void**>(m_instance) = m_new_vtable.get();
        }
    }

    ~VMT()
    {
        if (m_instance)
        {
            *static_cast<void**>(m_instance) = m_original_vtable;
        }
    }

    template<typename T>
    T Hook(size_t index, void* hook)
    {
        if (index < m_vtable_size)
        {
            T original = reinterpret_cast<T>(m_original_vtable[index]);
            m_new_vtable[index] = hook;
            return original;
        }
        return nullptr;
    }

    template<typename T>
    T Unhook(size_t index)
    {
        if (index < m_vtable_size)
        {
            T original = reinterpret_cast<T>(m_original_vtable[index]);
            m_new_vtable[index] = m_original_vtable[index];
            return original;
        }
        return nullptr;
    }

private:
    void* m_instance;
    void** m_vtable;
    void** m_original_vtable;
    size_t m_vtable_size;
    std::unique_ptr<void*[]> m_new_vtable;
};

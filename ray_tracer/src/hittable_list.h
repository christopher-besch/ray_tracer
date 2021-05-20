#pragma once

#include "hittable.h"

#include <memory>
#include <sstream>
#include <vector>

class HittableList : public Hittable
{
private:
    std::vector<Hittable*> m_objects;

public:
    HittableList() {}
    HittableList(Hittable* object)
    {
        add(object);
    }
    ~HittableList()
    {
        for (Hittable* object : m_objects)
            delete object;
    }

    // no marca, I won't mark these inline
    void clear()
    {
        m_objects.clear();
    }
    void add(Hittable* object)
    {
        m_objects.push_back(object);
    }

    virtual bool hit(const Ray& ray, double t_min, double t_max, HitRecord& record) const override;

    virtual std::string to_str() const override
    {
        std::stringstream buffer;
        for (int i = 0; i < m_objects.size(); ++i)
        {
            buffer << m_objects[i]->to_str();
            if (i != m_objects.size() - 1)
                buffer << '\n';
        }
        return buffer.str();
    }
};

#pragma once

#include "hittable.h"

#include <memory>
#include <vector>

class HittableList : public Hittable
{
private:
    std::vector<std::shared_ptr<Hittable>> m_objects;

public:
    HittableList() {}
    HittableList(std::shared_ptr<Hittable> object)
    {
        add(object);
    }

    // no marca, I won't mark these inline
    void clear()
    {
        m_objects.clear();
    }
    void add(std::shared_ptr<Hittable> object)
    {
        m_objects.push_back(object);
    }

    virtual bool hit(const Ray& ray, double t_min, double t_max, HitRecord& record) const override;
};

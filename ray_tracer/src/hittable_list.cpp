#include "hittable_list.h"

bool HittableList::hit(const Ray& ray, double t_min, double t_max, HitRecord& record) const
{
    HitRecord temp_record;
    bool      hit_anything    = false;
    double    current_closest = t_max;

    for (const std::shared_ptr<Hittable>& object : m_objects)
    {
        // only things in front of last hit or t_max are rendered
        if (object->hit(ray, t_min, current_closest, temp_record))
        {
            hit_anything    = true;
            current_closest = temp_record.distance;
            record          = temp_record;
        }
    }
    return hit_anything;
}

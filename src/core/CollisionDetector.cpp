#include "CollisionDetector.h"
#include <cassert>
#include <algorithm>

// Движемся из точки a в точку b и пытаемся подобрать точку c.
CollectionResult TryCollectPoint(const Point& a, const Point& b, const Point& c)
{
    // Проверим, что перемещение ненулевое.
    // Тут приходится использовать строгое равенство, а не приближённое,
    // пскольку при сборе заказов придётся учитывать перемещение даже на небольшое расстояние.
    assert (b.x != a.x or b.y != a.y);
    float u_x = c.x - a.x;
    float u_y = c.y - a.y;
    float v_x = b.x - a.x;
    float v_y = b.y - a.y;
    float u_dot_v = u_x * v_x + u_y * v_y;
    float u_len2 = u_x * u_x + u_y * u_y;
    float v_len2 = v_x * v_x + v_y * v_y;
    float proj_ratio = u_dot_v / v_len2;
    float sq_distance = u_len2 - (u_dot_v * u_dot_v) / v_len2;

    return CollectionResult(sq_distance, proj_ratio);
}

std::vector<GatheringEvent> find_gather_events(const ItemGathererProvider& provider)
{
    std::vector<GatheringEvent> events;

    float epsilon = 1e-10;

    for(int i = 0; i < provider.gatherers_count(); ++i)
    {
        Gatherer gath = provider.get_gatherer(i);

        if((std::abs(gath.start_pos.x - gath.end_pos.x) <= epsilon) &&
          (std::abs(gath.start_pos.y - gath.end_pos.y) <= epsilon))
        {
            continue;
        }
        for(int j = 0; j < provider.items_count(); ++j)
        {
            Item item = provider.get_item(j);
            auto result
                = TryCollectPoint(gath.start_pos, gath.end_pos, item.position);

            if (result.is_collected(gath.width + item.width))
            {
                GatheringEvent evt{j, i, result.sq_distance, result.proj_ratio};
                events.push_back(evt);
            }
        }
    }

    std::sort(events.begin(), events.end(), [](const GatheringEvent& evt1, const GatheringEvent& evt2)
    										{
                  	  	  	  	  	  	  		return evt1.time < evt2.time;
    										});

    return events;
}

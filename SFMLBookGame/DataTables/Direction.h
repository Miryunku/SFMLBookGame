#pragma once

struct Direction {
    Direction(float aangle, float ddistance)
        : angle {aangle},
        distance {ddistance}
    {
        //empty by default
    }

    float angle;
    float distance;
};

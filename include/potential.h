#ifndef MOPOTENTIAL_H
#define MOPOTENTIAL_H

namespace Mo {

class Potential {
public:
    virtual ~Potential() {}
    /**
     * @brief Evaluate the potential between two positions
     * @param x1 First position
     * @param x2 Second position
     * @return The relative interaction potential between x1 and x2.
     */
    virtual float operator()(const float* x1, const float* x2) = 0;

    /**
     * @brief The range of the potential.
     *
     * Interactions can be neglected beyond the range of the potential.  A negative
     * range indicates infinite range.
     *
     * @return The range of the potential in world coordinates.
     */
    virtual float range() const = 0;
};

}

#endif // MOPOTENTIAL_H

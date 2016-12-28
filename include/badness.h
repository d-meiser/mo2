#ifndef BADNESS_H
#define BADNESS_H


namespace Mo {

class Mosaic;
class TargetImage;


class Badness {
public:
    virtual ~Badness() {}

    /**
     * @brief Calculate the badness of a mosaic
     * @param model the model
     * @return the badness of the mosaic
     */
    virtual float computeBadness(const Mosaic& mosaic) = 0;
};

}

#endif // BADNESS_H

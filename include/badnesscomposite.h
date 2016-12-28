#ifndef BADNESSCOMPOSITE_H
#define BADNESSCOMPOSITE_H


#include <memory>
#include <vector>

#include <badness.h>
#include <mo_lib_export.h>


namespace Mo {

class MO_LIB_EXPORT BadnessComposite : public Badness {
public:
    virtual ~BadnessComposite();
    virtual float computeBadness(const Mosaic &model);
    void addBadness(std::unique_ptr<Badness> badness);

private:
    std::vector<std::unique_ptr<Badness> > badnesses_;
};

}

#endif // BADNESSCOMPOSITE_H

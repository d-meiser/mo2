#include <badnesscomposite.h>


namespace Mo {

BadnessComposite::~BadnessComposite() {}

float BadnessComposite::computeBadness(const Mosaic &model) {
    float badness = 0.0f;
    for (auto& b : badnesses_) {
        badness += b->computeBadness(model);
    }
    return badness;
}

void BadnessComposite::addBadness(std::unique_ptr<Badness> badness) {
    badnesses_.emplace_back(std::move(badness));
}

}


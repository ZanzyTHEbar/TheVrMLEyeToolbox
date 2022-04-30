#ifndef RANSAC_HPP
#define RANSAC_HPP
#include "defines.hpp"

struct Data
{

};

extern Data data;

class Ransac
{
public:
    Ransac();
    virtual ~Ransac();

    Data fitRotatedEllipse();
    Data fitRotatedEllipseRansac(double data[]);

private:
};
extern Ransac ransac;
#endif // RANSAC_HPP
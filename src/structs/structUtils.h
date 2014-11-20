#ifndef STRUCT_UTILS
#define STUCT_UTILS

Tuple createTuple(float _x, float _y, float _z){
    struct Tuple myTuple;
    myTuple.x = _x;
    myTuple.y = _y;
    myTuple.z = _z;
    return myTuple;
}

#endif


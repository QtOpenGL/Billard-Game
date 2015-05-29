#ifndef POLYGON
#define POLYGON

#include <utility>

template <class Vecteur>
struct Quadrilatere{
    Vecteur top_left_corner     {-1, +1, 0};
    Vecteur top_right_corner    {+1, +1, 0};
    Vecteur bottom_left_corner  {-1, -1, 0};
    Vecteur bottom_right_corner {+1, -1, 0};

    Quadrilatere() = default;
    Quadrilatere(Vecteur tlc, Vecteur trc, Vecteur blc, Vecteur brc)
        : top_left_corner    {std::move(tlc)}
        , top_right_corner   {std::move(trc)}
        , bottom_left_corner {std::move(blc)}
        , bottom_right_corner{std::move(brc)}
    {}


    void translate(Vecteur const& translation)
    {
        top_left_corner  += translation;
        top_right_corner += translation;
        bottom_left_corner  += translation;
        bottom_right_corner += translation;
    }
};
template <class T>
struct Quadriedre{
    Quadrilatere<T> face_superieure;
    Quadrilatere<T> face_inferieure;
};

#endif // POLYGON


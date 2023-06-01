#include "triangulation/detail/linkbuilder-impl.h"

#include "triangulation/dim2/triangulation2.h"
#include "triangulation/dim3/triangulation3.h"
#include "triangulation/dim4/triangulation4.h"
#include "triangulation/generic/triangulation.h"

namespace regina::detail {

template class LinkBuilder<3, 0>;
template class LinkBuilder<4, 0>;
template class LinkBuilder<4, 1>;
template class LinkBuilder<5, 0>;
template class LinkBuilder<5, 1>;
template class LinkBuilder<5, 2>;
template class LinkBuilder<6, 0>;
template class LinkBuilder<6, 1>;
template class LinkBuilder<6, 2>;
template class LinkBuilder<6, 3>;
template class LinkBuilder<7, 0>;
template class LinkBuilder<7, 1>;
template class LinkBuilder<7, 2>;
template class LinkBuilder<7, 3>;
template class LinkBuilder<7, 4>;
template class LinkBuilder<8, 0>;
template class LinkBuilder<8, 1>;
template class LinkBuilder<8, 2>;
template class LinkBuilder<8, 3>;
template class LinkBuilder<8, 4>;
template class LinkBuilder<8, 5>;

#ifdef REGINA_HIGHDIM
template class LinkBuilder<9, 0>;
template class LinkBuilder<9, 1>;
template class LinkBuilder<9, 2>;
template class LinkBuilder<9, 3>;
template class LinkBuilder<9, 4>;
template class LinkBuilder<9, 5>;
template class LinkBuilder<9, 6>;
template class LinkBuilder<10, 0>;
template class LinkBuilder<10, 1>;
template class LinkBuilder<10, 2>;
template class LinkBuilder<10, 3>;
template class LinkBuilder<10, 4>;
template class LinkBuilder<10, 5>;
template class LinkBuilder<10, 6>;
template class LinkBuilder<10, 7>;
template class LinkBuilder<11, 0>;
template class LinkBuilder<11, 1>;
template class LinkBuilder<11, 2>;
template class LinkBuilder<11, 3>;
template class LinkBuilder<11, 4>;
template class LinkBuilder<11, 5>;
template class LinkBuilder<11, 6>;
template class LinkBuilder<11, 7>;
template class LinkBuilder<11, 8>;
template class LinkBuilder<12, 0>;
template class LinkBuilder<12, 1>;
template class LinkBuilder<12, 2>;
template class LinkBuilder<12, 3>;
template class LinkBuilder<12, 4>;
template class LinkBuilder<12, 5>;
template class LinkBuilder<12, 6>;
template class LinkBuilder<12, 7>;
template class LinkBuilder<12, 8>;
template class LinkBuilder<12, 9>;
template class LinkBuilder<13, 0>;
template class LinkBuilder<13, 1>;
template class LinkBuilder<13, 2>;
template class LinkBuilder<13, 3>;
template class LinkBuilder<13, 4>;
template class LinkBuilder<13, 5>;
template class LinkBuilder<13, 6>;
template class LinkBuilder<13, 7>;
template class LinkBuilder<13, 8>;
template class LinkBuilder<13, 9>;
template class LinkBuilder<13, 10>;
template class LinkBuilder<14, 0>;
template class LinkBuilder<14, 1>;
template class LinkBuilder<14, 2>;
template class LinkBuilder<14, 3>;
template class LinkBuilder<14, 4>;
template class LinkBuilder<14, 5>;
template class LinkBuilder<14, 6>;
template class LinkBuilder<14, 7>;
template class LinkBuilder<14, 8>;
template class LinkBuilder<14, 9>;
template class LinkBuilder<14, 10>;
template class LinkBuilder<14, 11>;
template class LinkBuilder<15, 0>;
template class LinkBuilder<15, 1>;
template class LinkBuilder<15, 2>;
template class LinkBuilder<15, 3>;
template class LinkBuilder<15, 4>;
template class LinkBuilder<15, 5>;
template class LinkBuilder<15, 6>;
template class LinkBuilder<15, 7>;
template class LinkBuilder<15, 8>;
template class LinkBuilder<15, 9>;
template class LinkBuilder<15, 10>;
template class LinkBuilder<15, 11>;
template class LinkBuilder<15, 12>;

#endif

}

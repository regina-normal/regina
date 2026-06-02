
#include "algebra/grouppresentation.h"
#include "triangulation/dim3.h"

int main(int argc, char* argv[]) {
    // This is Alex He's difficult 3-sphere, for which the algebra tab in
    // Regina 7.x "became a death trap".  Ouch.
    auto t = regina::Triangulation<3>::fromSig("-cucLLvLLwvwMvzzvPQwwvQPAAMzwQLAAQQQLQvwvQvLvPMzMPALwMMvLzzvLvMvLvAQzQvQzvwwAwzLQAAPQQMvPMLPvQQwQAMQQQkbadaiaiahanakasaqapaBazasaCavaBaFaJaEaAaKaEaLaDaBaJaEaOaOaGaPaPaMaRaTaQaLaSaPaVaQaOaRaVaTaWaWaXaWaZaYa1a3a4a6a6a+aab9aabebhbbbebcbjbhblbhbnbobnbobmbubybAbAbGbLbxbQbEbIbQbRbTbIbGbWbEbVbNbYbYbNbMbYbPb3b+b3bZbSbTb8bWbXbdcccccdc4b0bbc+b5bgcbchc-b-b-bicocacgcbclcdcncjchcicjcpcqcqcmctcoctcocscpcscrcsctcdwrrrttwvjuqcpdmiloslbpgktvquvddtwkjobsqnvbtvpldkvnhaierpssvcpbwrtiajxaaxspaextpbxrwcvhwcdwqgluvvuniliqdonnbxphhnbmfkaxloowhxbpeqkovfovbddfxcivqwqsrf");
    auto g = t.group();
    std::cout << g.countGenerators() << ' ' << g.countRelations() << std::endl;
    return 0;
}

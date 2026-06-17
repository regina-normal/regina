
#include "algebra/grouppresentation.h"
#include "triangulation/dim3.h"

// These are first-generation isomorphism signatures for Alex He's difficult
// 3-spheres, for which the algebra tab in Regina 7.x "became a death trap".
//
std::array<std::string, 3> isoSigs {
    "-cucLLvLLwvwMvzzvPQwwvQPAAMzwQLAAQQQLQvwvQvLvPMzMPALwMMvLzzvLvMvLvAQzQvQzvwwAwzLQAAPQQMvPMLPvQQwQAMQQQkbadaiaiahanakasaqapaBazasaCavaBaFaJaEaAaKaEaLaDaBaJaEaOaOaGaPaPaMaRaTaQaLaSaPaVaQaOaRaVaTaWaWaXaWaZaYa1a3a4a6a6a+aab9aabebhbbbebcbjbhblbhbnbobnbobmbubybAbAbGbLbxbQbEbIbQbRbTbIbGbWbEbVbNbYbYbNbMbYbPb3b+b3bZbSbTb8bWbXbdcccccdc4b0bbc+b5bgcbchc-b-b-bicocacgcbclcdcncjchcicjcpcqcqcmctcoctcocscpcscrcsctcdwrrrttwvjuqcpdmiloslbpgktvquvddtwkjobsqnvbtvpldkvnhaierpssvcpbwrtiajxaaxspaextpbxrwcvhwcdwqgluvvuniliqdonnbxphhnbmfkaxloowhxbpeqkovfovbddfxcivqwqsrf",
    "-cweLLvLLLvAMLzvzLwLvwzvvLLwPMPQQPMAAvvQwwPwAQQvPvAMwzvvQALPPLMPzLwvzPzvvMLwwLQPvLzAQQQzQMvMAvQPvMvLPAAAvQMwzLzMMQPzzPQMzwvvAvzzLAQzAvQMMAAAPwPLzAAAQMPAQzLzQQQPPLMwLwPzvwwQLPQwvLLwQMLQQMAQQcbadaiahamaiaoakarasapauayaxaEaCaBaOaYaOaJaQaIaGaHaNaKaLaMaPaQaQa4a1a0aNa4a5a6a2aVa7aab7aab1aYaZa3a3a4ahb6a5aibjbkbdbcbbbfbnblbsbmbibjbjbobAbFbEbvbsbybrbwbvbIbybJbzbFbSbRbEbVb4b2bJbYbObNb5bOb9bacbc-bicWb6bacYb6bhcbc0bZb4bcc5bdc5bdc8bqc7bic-becfcecfcdcycBcAckcFcHcGcmcncIcycLcNcMcLctcscxcRcUcVcPcCcKcIcJcGcFcDcDc1cZcJcJcZcKcZcVcUcTc-c4c+c5c4ced6c3ceded4c3cpdddtdudsdrdsdadjdldidkdydwdddzdzdidhdsdxdwdndmdvdrdHdAdqdudtdCdAdAdMdIdydNdzdCdPdIdFdGdTdHdNdJdMdNdTdSdLdUdTdXdRdWdYd1d2d3dXdZd8d2d5d6d8d7d9dfeae+deefelecenekeoejekeieteperesememeseoeneteqeqeveveueseuevedwrjafptwcjihafnvmphuapvnpihavggmonouhtgcdaaekscnnuhmavrajfocudetsorkbaxwfkcsghvtgtqatbxhsrwougldixavahgaiukcpdthehoqjonfjjjvhhxnuaxoothqaqarfjtaqhjrrnrwnnaaosuodosreshifhmwhqbbikqaladrkwswiqkaigwdpufwheqhgludrrljfagnikwkakiwmhxdgeflvapldlnniobiqmfsvoboavmqxduftscankacjmkkxxqqgw",
    "-cxeLLvLLLvMMwzvvzwwvAPwvLAMvvwMwPMMAMwLLMQLwQQQQAzLzLLLwPwALPwvMPMzALPAMAvwPLALMLLwvPPwAPzwzPAQvLwMMMwPAAAvQMvvAwzMQAQPLwLMLzPzPzzMPLvwMzMLzLMQvAvwQAvwzvLPzLPQLLQzPPwvwzLQLMPwQQPQQQQQQMAQQQbadaiahamaiaoakaoapapaxataAaHaLaBaCaDaQaEaGaCaSaZa3aVaWa5aMa6a6a9a2a5a+aUaTaSaUafbgb+aWa0abbZa8a2a7a+a5a9alb4a8akb6albkb9acbnbjbpbqbhbhbqbwbAblbvbCbybCbwbDbHbEbNbBbEbPbybQbAbTbFbKbLbPbGbQbKbJbOb0bYbPbSb0b7bUb+b0b+bXbhc6bacbc4b3blc5b+bcc8brcbcocccpckcvcqcAcqcBcjcmcocncvcncIcFcHcHcGcsctcxcwcNcGcMczcLcUcCcYcJcFcIc1c1cJc1cLcMcWcYc3c3c2c7cUcScVcadcd8c8cZc3chded6ckdldedgdjd-cudbdddddgdmdidrdAdqdodndCdHdKdGdDdNdLdMdRdSdQdzdQdEdCdNdKd6dGdIdId7d3d+d9d6d3d-d9d+dRdTdiegedeZdYdkefeqerelemefeteqede-d8dbeae+d9d8d-dueuebececeuenepemeoesemepenetekeoevetererepeoesewewewedwrjafutnwjadhcdvccasonhtlfoakieajxqnowcdahsfikdktmdmdouutkvbgabixkbpiabjqvagmppmcpabmkarsjjwsssoqpwsqmouleoacjjvrabbovaomclxmkxbajnwnvniheaevwduaprfrljlofglivhfwuldqragrsalxxonaquladcjcqwsojwnumlqxqwarndqaadevhfbealsxmbjwrcwpkknnpahcgadumxqmqhuubelrwrxqqvnvjjrfnnowowgvdojjfwkewo"
};

// These are second-generation signatures for the same difficult 3-spheres.
// The simplification times differ here, presumably as a side-effect of
// changes in the triangulation labellings.
//
std::array<std::string, 3> neoSigs {
    "-cuc3admGHhuWIkAqckgHdqczdncXKruXLvyjMzs5KuQHKsGXeyGbfzEzLxCrizg6gF6HNJuAjN8PkFeYkHUIOHAIiJOAQMIQQLwYPSKcRQKAlUWkmVWQRWa7QU0cSVcJlVeRmXkRmYmtT0CBn5UdV42BU6ceW+6tp-qSW-uSWcjCWdfuqenKXerSrhx8XiBeYhNuYiLCYj-31t-ZLs1inrqYisV-5-J1SE9xJh0aQJz6FgL4DdkfbGzLaQayNfXajaWzqn3R2J2CSvRbTD2PgrKesjKmnykCxCwGQGujRSKtmYQktcNzsDVWaBrqvzPzgkVcQrnsgdhZo1aapcVSstV4mC1LWAm",
    "-cwe3agWamickWcnicsqduWdI4dJifyydCyeEqfEye0yg3yg0if8qihPfVGjlPiobjwrhybhA5k-WhHrmKPlejkfjmqXiuXjnbpXroUPnZzkWHoFbrArrCPrq6lKXsO5my6qzItSHnxYsh6sw6qz6n-zubQp4zpLQoeQugAuIspIAu6rqLcv9XqOkvMkub6uPYvSkwVswkQsuQwmcx7IssssDQw-Qw+sxbBtCcuFIzgJuOkwQ6vWkwZAv0YvYYvsdw0Aw0Yx86AARAd7xAdByZxbBzcdzgRzBZyidzGlzkZzI7zllCJ7zGlCJlAMJARlBDlBytBUJBUdET7BW7DTdEKJCKBCXJERtE67E4RFV7DZ7F2BGgSF9ZF8JGgSGaCGbeHaeHeCHjeInCIt0HsKHmmIruIpeIuCIuSIvSI3-3xgYVphu47iI4-N7jQ033o-59c1Z-pROIYDPbg9Q6e+v--Uk-JU0anfocJ6uaW5wGhMGYeSg4Gsd4Gbh1GSGMIIbubWolobcZueqqniLgSHaZGRnco4S4fdy5SMLcixqkiyEJvHPsosvbGtQazQm6aHTrG6wwcrbyifSZUyK7srCeogQbItbcSBc6qYnLvHAtvWC4GWcur7GdUyz7bwrPqWDiiJojcto6ydizQJckvLfYMdIYiqmfnQHxGRKtwrgwTeOcDie6D7GKDNGWAacd",
    "-cxe3agWapqbxOcticyqdy4cs4dJOdFaezidPafGGfOOfXWeKGgLWfVWeWyg0ifTyf5GgW4fX4f5WgZWg7ai-OhbjjarihjiiXiiHikPjpHjhjjnPjrzkujkurkxPkyHkxPlzzlKHlMjmGjmObnJ5nLXmZPnTrpbIocQoo6o4zsaYp-Psy6sBsttAqiQqkQrhcrkQuksuJ6rokvGAvwsstQtEQsVcwDYvCswWYw3YvHAvIAx7AxOIvPQuNsvMsvSkvSIx8Ix6Yxa7w0Qy5szdtzpdyqtAeBzwJyjdzoZAA7ABlBFlCzlAutDvBCGBDJlByZBDJCDZBH7DKRDMRCLdEUZCLJCWdEPtERJE5ZDV7D57E5ZF8dFaeG87GdKGeCG-ZGfmHlCHpKHiSHouHrKIveIoeIsuIuCIu0Iv0Iw83-CJ3G3Jgod+Kq-33U3LepWDmtvcnjfg93-FLk0RUxvCCL70WbMiqgMaiMCGxLil47CHTX-GK4gFVw3xbXUycvRyfaQayNfdCWbNlvAMjbqbquHm1UDpCXbodu4UDXfbRQaLclOmbWTqeHSkKa1YLWCcWqcJBX6sbklRjkOgzNaGaHBnrVeQTkTevhAmyIKaCwDrasO0RYRYBbReGStalZbmGgDetGv3IqKw9OZmtiRwIwXoWKnqbRIqaZcrWgBojQPksacv5gn1aasqYssluqXa"
};

void process(const std::string& sig) {
    auto t = regina::Triangulation<3>::fromSig(sig);
    auto g = t.group();
    std::cout << g.countGenerators() << ' ' << g.countRelations() << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        for (const auto& s : isoSigs)
            process(s);
    } else {
        for (int i = 1; i < argc; ++i) {
            int which;
            try {
                which = regina::parse<int>(argv[i]);
                if (which < 0 || which >= isoSigs.size()) {
                    std::cerr << "ERROR: Invalid index: " << which << std::endl;
                    std::cerr << "Valid indices are 0.." << (isoSigs.size() - 1)
                        << std::endl;
                    continue;
                }
            } catch (const regina::InvalidArgument&) {
                std::cerr << "ERROR: Invalid index: " << argv[i] << std::endl;
                std::cerr << "Valid indices are 0.." << (isoSigs.size() - 1)
                    << std::endl;
                continue;
            }
            process(isoSigs[which]);
        }
    }
    return 0;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 19:47:26 by llelievr          #+#    #+#             */
/*   Updated: 2019/05/01 17:02:57 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

t_line	lines[] = {
	{.a = {1056, 571}, .b = {1236, 600}}, {.a = {500, 500}, .b = {1117, 591}}, {.a = {1108, 592}, .b = {1132, 598}}, {.a = {1165, 607}, .b = {1075, 598}}, {.a = {868, 565}, .b = {993, 591}}, {.a = {1411, 676}, .b = {779, 555}}, {.a = {946, 590}, .b = {1187, 644}}, {.a = {743, 552}, .b = {500, 500}}, {.a = {813, 572}, .b = {822, 576}}, {.a = {1321, 697}, .b = {714, 553}}, {.a = {1006, 629}, .b = {777, 573}}, {.a = {731, 563}, .b = {1215, 698}}, {.a = {1139, 683}, .b = {1001, 645}}, {.a = {904, 619}, .b = {1089, 676}}, {.a = {1167, 704}, .b = {892, 623}}, {.a = {510, 503}, .b = {837, 612}}, {.a = {1020, 676}, .b = {748, 586}}, {.a = {1142, 727}, .b = {1001, 679}}, {.a = {1104, 722}, .b = {986, 683}}, {.a = {816, 620}, .b = {541, 516}}, {.a = {638, 555}, .b = {957, 685}}, {.a = {890, 660}, .b = {1247, 810}}, {.a = {1313, 846}, .b = {700, 586}}, {.a = {927, 689}, .b = {671, 577}}, {.a = {879, 676}, .b = {909, 694}}, {.a = {1253, 862}, .b = {1186, 834}}, {.a = {1320, 903}, .b = {931, 714}}, {.a = {819, 660}, .b = {1153, 833}}, {.a = {1248, 889}, .b = {1095, 812}}, {.a = {1126, 832}, .b = {500, 500}}, {.a = {767, 646}, .b = {510, 505}}, {.a = {790, 662}, .b = {683, 604}}, {.a = {951, 759}, .b = {945, 759}}, {.a = {632, 578}, .b = {665, 599}}, {.a = {1055, 838}, .b = {1101, 870}}, {.a = {1151, 906}, .b = {935, 777}}, {.a = {1177, 937}, .b = {638, 590}}, {.a = {1030, 849}, .b = {792, 694}}, {.a = {665, 610}, .b = {829, 723}}, {.a = {500, 500}, .b = {945, 808}}, {.a = {838, 737}, .b = {1166, 975}}, {.a = {972, 842}, .b = {1056, 906}}, {.a = {1023, 886}, .b = {1238, 1051}}, {.a = {986, 870}, .b = {931, 832}}, {.a = {637, 606}, .b = {882, 801}}, {.a = {1193, 1051}, .b = {581, 565}}, {.a = {890, 818}, .b = {679, 647}}, {.a = {1270, 1137}, .b = {1047, 956}}, {.a = {1067, 979}, .b = {878, 823}}, {.a = {956, 893}, .b = {651, 631}}, {.a = {616, 603}, .b = {974, 926}}, {.a = {917, 882}, .b = {1141, 1092}}, {.a = {660, 650}, .b = {841, 824}}, {.a = {752, 742}, .b = {713, 705}}, {.a = {1060, 1048}, .b = {1038, 1032}}, {.a = {1100, 1099}, .b = {1066, 1070}}, {.a = {930, 937}, .b = {668, 673}}, {.a = {797, 811}, .b = {1120, 1159}}, {.a = {994, 1032}, .b = {840, 870}}, {.a = {722, 746}, .b = {935, 989}}, {.a = {669, 691}, .b = {654, 678}}, {.a = {700, 734}, .b = {943, 1025}}, {.a = {858, 931}, .b = {892, 974}}, {.a = {693, 737}, .b = {912, 1012}}, {.a = {512, 515}, .b = {961, 1090}}, {.a = {649, 693}, .b = {714, 781}}, {.a = {929, 1069}, .b = {796, 897}}, {.a = {773, 873}, .b = {667, 733}}, {.a = {904, 1073}, .b = {1010, 1229}}, {.a = {877, 1045}, .b = {647, 716}}, {.a = {936, 1154}, .b = {866, 1054}}, {.a = {773, 918}, .b = {769, 919}}, {.a = {906, 1142}, .b = {729, 868}}, {.a = {863, 1092}, .b = {727, 873}}, {.a = {683, 805}, .b = {800, 1007}}, {.a = {738, 910}, .b = {893, 1182}}, {.a = {805, 1035}, .b = {660, 785}}, {.a = {655, 778}, .b = {690, 846}}, {.a = {782, 1024}, .b = {754, 983}}, {.a = {670, 826}, .b = {932, 1339}}, {.a = {554, 607}, .b = {832, 1163}}, {.a = {843, 1192}, .b = {717, 946}}, {.a = {820, 1167}, .b = {689, 901}}, {.a = {671, 867}, .b = {651, 829}}, {.a = {691, 920}, .b = {610, 747}}, {.a = {772, 1125}, .b = {561, 643}}, {.a = {805, 1230}, .b = {732, 1061}}, {.a = {657, 887}, .b = {500, 500}}, {.a = {792, 1240}, .b = {613, 792}}, {.a = {518, 549}, .b = {554, 646}}, {.a = {647, 908}, .b = {662, 958}}, {.a = {655, 944}, .b = {669, 994}}, {.a = {817, 1448}, .b = {672, 1029}}, {.a = {640, 940}, .b = {689, 1116}}, {.a = {606, 856}, .b = {571, 745}}, {.a = {687, 1153}, .b = {626, 954}}, {.a = {567, 750}, .b = {669, 1136}}, {.a = {659, 1108}, .b = {646, 1069}}, {.a = {626, 1002}, .b = {621, 999}}, {.a = {500, 500}, .b = {581, 854}}, {.a = {668, 1242}, .b = {647, 1162}}, {.a = {695, 1391}, .b = {562, 792}}, {.a = {605, 1007}, .b = {602, 1005}}, {.a = {640, 1222}, .b = {590, 978}}, {.a = {519, 604}, .b = {617, 1165}}, {.a = {577, 950}, .b = {565, 896}}, {.a = {617, 1236}, .b = {639, 1404}}, {.a = {534, 737}, .b = {561, 929}}, {.a = {593, 1172}, .b = {551, 894}}, {.a = {500, 500}, .b = {577, 1179}}, {.a = {572, 1172}, .b = {532, 828}}, {.a = {521, 730}, .b = {576, 1346}}, {.a = {565, 1255}, .b = {567, 1340}}, {.a = {530, 929}, .b = {511, 675}}, {.a = {540, 1183}, .b = {540, 1293}}, {.a = {517, 907}, .b = {534, 1419}}, {.a = {500, 500}, .b = {510, 935}}, {.a = {507, 992}, .b = {508, 1299}}, {.a = {503, 1023}, .b = {499, 983}}, {.a = {496, 895}, .b = {489, 1283}}, {.a = {490, 1054}, .b = {480, 1303}}, {.a = {490, 793}, .b = {472, 1173}}, {.a = {489, 738}, .b = {473, 1014}}, {.a = {455, 1277}, .b = {466, 1018}}, {.a = {468, 925}, .b = {463, 944}}, {.a = {451, 1049}, .b = {455, 966}}, {.a = {412, 1370}, .b = {459, 866}}, {.a = {447, 965}, .b = {500, 500}}, {.a = {441, 957}, .b = {433, 993}}, {.a = {396, 1238}, .b = {431, 974}}, {.a = {431, 943}, .b = {463, 732}}, {.a = {437, 878}, .b = {482, 603}}, {.a = {381, 1166}, .b = {384, 1117}}, {.a = {385, 1093}, .b = {319, 1412}}, {.a = {428, 850}, .b = {389, 1026}}, {.a = {370, 1097}, .b = {399, 948}}, {.a = {452, 706}, .b = {426, 813}}, {.a = {470, 623}, .b = {393, 930}}, {.a = {389, 928}, .b = {350, 1069}}, {.a = {368, 982}, .b = {432, 740}}, {.a = {270, 1294}, .b = {300, 1180}}, {.a = {472, 593}, .b = {344, 1011}}, {.a = {338, 1018}, .b = {459, 628}}, {.a = {277, 1182}, .b = {300, 1096}}, {.a = {285, 1125}, .b = {337, 961}}, {.a = {397, 789}, .b = {363, 875}}, {.a = {181, 1364}, .b = {206, 1277}}, {.a = {312, 989}, .b = {500, 500}}, {.a = {262, 1099}, .b = {206, 1230}}, {.a = {265, 1069}, .b = {308, 962}}, {.a = {415, 700}, .b = {356, 836}}, {.a = {130, 1359}, .b = {239, 1091}}, {.a = {187, 1199}, .b = {429, 654}}, {.a = {80, 1402}, .b = {293, 934}}, {.a = {295, 923}, .b = {270, 962}}, {.a = {348, 802}, .b = {131, 1226}}, {.a = {65, 1348}, .b = {187, 1095}}, {.a = {127, 1199}, .b = {385, 712}}, {.a = {244, 964}, .b = {273, 908}}, {.a = {500, 500}, .b = {127, 1149}}, {.a = {406, 659}, .b = {117, 1144}}, {.a = {45, 1256}, .b = {242, 919}}, {.a = {276, 859}, .b = {332, 767}}, {.a = {200, 973}, .b = {193, 975}}, {.a = {152, 1036}, .b = {139, 1044}}, {.a = {290, 812}, .b = {332, 746}}, {.a = {237, 880}, .b = {-32, 1260}}, {.a = {64, 1113}, .b = {120, 1024}}, {.a = {114, 1025}, .b = {240, 849}}, {.a = {244, 839}, .b = {329, 724}}, {.a = {289, 774}, .b = {168, 927}}, {.a = {99, 1013}, .b = {165, 920}}, {.a = {230, 833}, .b = {51, 1044}}, {.a = {234, 817}, .b = {250, 795}}, {.a = {297, 737}, .b = {372, 647}}, {.a = {-87, 1166}, .b = {483, 518}}, {.a = {63, 982}, .b = {-55, 1102}}, {.a = {84, 946}, .b = {-144, 1182}}, {.a = {61, 959}, .b = {168, 843}}, {.a = {59, 951}, .b = {254, 748}}, {.a = {-35, 1035}, .b = {-15, 1008}}, {.a = {195, 797}, .b = {-150, 1130}}, {.a = {332, 661}, .b = {500, 500}}, {.a = {331, 657}, .b = {-157, 1104}}, {.a = {34, 925}, .b = {189, 779}}, {.a = {404, 585}, .b = {114, 839}}, {.a = {21, 915}, .b = {292, 678}}, {.a = {-39, 956}, .b = {478, 517}}, {.a = {7, 904}, .b = {89, 832}}, {.a = {48, 860}, .b = {62, 845}}, {.a = {13, 878}, .b = {299, 653}}, {.a = {-81, 941}, .b = {-124, 965}}, {.a = {3, 862}, .b = {27, 841}}, {.a = {-153, 968}, .b = {255, 673}}, {.a = {172, 727}, .b = {-100, 910}}, {.a = {231, 679}, .b = {137, 739}}, {.a = {182, 706}, .b = {-22, 835}}, {.a = {-172, 923}, .b = {366, 582}}, {.a = {43, 781}, .b = {-156, 897}}, {.a = {181, 689}, .b = {76, 749}}, {.a = {342, 591}, .b = {-44, 811}}, {.a = {-280, 941}, .b = {222, 654}}, {.a = {39, 753}, .b = {158, 684}}, {.a = {226, 644}, .b = {500, 500}}, {.a = {-172, 844}, .b = {107, 697}}, {.a = {-95, 795}, .b = {-348, 914}}, {.a = {-401, 932}, .b = {-106, 788}}, {.a = {-261, 854}, .b = {30, 715}}, {.a = {95, 681}, .b = {86, 681}}, {.a = {202, 628}, .b = {500, 500}}, {.a = {-169, 781}, .b = {-69, 733}}, {.a = {-230, 795}, .b = {31, 686}}, {.a = {11, 691}, .b = {86, 660}}, {.a = {-103, 729}, .b = {-28, 697}}, {.a = {-57, 703}, .b = {59, 656}}, {.a = {144, 623}, .b = {222, 595}}, {.a = {-194, 732}, .b = {315, 560}}, {.a = {301, 564}, .b = {-169, 709}}, {.a = {145, 609}, .b = {337, 549}}, {.a = {-185, 703}, .b = {-224, 710}}, {.a = {229, 576}, .b = {383, 532}}, {.a = {33, 627}, .b = {-307, 711}}, {.a = {11, 624}, .b = {434, 516}}, {.a = {-328, 697}, .b = {295, 547}}, {.a = {359, 531}, .b = {-216, 657}}, {.a = {126, 579}, .b = {-395, 686}}, {.a = {-68, 615}, .b = {-77, 613}}, {.a = {-33, 599}, .b = {-229, 629}}, {.a = {-335, 641}, .b = {500, 500}}, {.a = {-249, 617}, .b = {-189, 605}}, {.a = {500, 500}, .b = {-105, 587}}, {.a = {352, 520}, .b = {-310, 608}}, {.a = {329, 521}, .b = {-233, 590}}, {.a = {-368, 599}, .b = {331, 518}}, {.a = {-135, 565}, .b = {-179, 563}}, {.a = {-140, 556}, .b = {163, 527}}, {.a = {203, 522}, .b = {46, 529}}, {.a = {85, 525}, .b = {194, 516}}, {.a = {31, 522}, .b = {-421, 536}}, {.a = {233, 508}, .b = {-447, 527}}, {.a = {238, 506}, .b = {-10, 509}}, {.a = {-241, 506}, .b = {-211, 500}}, {.a = {-186, 495}, .b = {-37, 492}}, {.a = {-196, 485}, .b = {185, 490}}, {.a = {79, 484}, .b = {-192, 469}}, {.a = {-1, 474}, .b = {138, 479}}, {.a = {98, 475}, .b = {146, 476}}, {.a = {-284, 443}, .b = {-390, 429}}, {.a = {-218, 439}, .b = {168, 470}}, {.a = {224, 474}, .b = {-494, 400}}, {.a = {-282, 418}, .b = {76, 453}}, {.a = {17, 443}, .b = {-79, 427}}, {.a = {251, 467}, .b = {422, 489}}, {.a = {98, 442}, .b = {-334, 376}}, {.a = {-243, 385}, .b = {18, 423}}, {.a = {311, 468}, .b = {-348, 353}}, {.a = {-42, 402}, .b = {257, 454}}, {.a = {348, 470}, .b = {-300, 338}}, {.a = {-166, 361}, .b = {-330, 319}}, {.a = {-92, 367}, .b = {-144, 352}}, {.a = {89, 402}, .b = {258, 441}}, {.a = {-97, 351}, .b = {170, 415}}, {.a = {160, 411}, .b = {178, 414}}, {.a = {-226, 302}, .b = {-51, 347}}, {.a = {102, 388}, .b = {-191, 298}}, {.a = {477, 493}, .b = {-180, 295}}, {.a = {-15, 343}, .b = {90, 371}}, {.a = {362, 456}, .b = {239, 415}}, {.a = {264, 421}, .b = {138, 377}}, {.a = {104, 363}, .b = {-31, 310}}, {.a = {-23, 311}, .b = {-25, 307}}, {.a = {-243, 223}, .b = {-268, 209}}, {.a = {48, 327}, .b = {102, 345}}, {.a = {-45, 283}, .b = {-140, 240}}, {.a = {-39, 276}, .b = {50, 310}}, {.a = {347, 434}, .b = {-385, 113}}, {.a = {146, 342}, .b = {343, 429}}, {.a = {-220, 171}, .b = {-303, 125}}, {.a = {297, 404}, .b = {209, 361}}, {.a = {-107, 203}, .b = {-393, 60}}, {.a = {-141, 177}, .b = {-116, 182}}, {.a = {-387, 38}, .b = {295, 392}}, {.a = {-64, 197}, .b = {-56, 198}}, {.a = {462, 479}, .b = {-93, 171}}, {.a = {-191, 110}, .b = {363, 421}}, {.a = {-65, 172}, .b = {145, 291}}, {.a = {121, 273}, .b = {-75, 148}}, {.a = {-349, -28}, .b = {-232, 37}}, {.a = {91, 237}, .b = {500, 500}}, {.a = {267, 346}, .b = {-101, 99}}, {.a = {-327, -61}, .b = {206, 297}}, {.a = {35, 175}, .b = {-4, 140}}, {.a = {371, 407}, .b = {278, 338}}, {.a = {418, 439}, .b = {286, 339}}, {.a = {313, 357}, .b = {-30, 87}}, {.a = {-22, 87}, .b = {-88, 32}}, {.a = {82, 165}, .b = {298, 335}}, {.a = {94, 163}, .b = {67, 135}}, {.a = {223, 263}, .b = {396, 410}}, {.a = {-38, 29}, .b = {500, 500}}, {.a = {34, 83}, .b = {195, 224}}, {.a = {125, 155}, .b = {134, 160}}, {.a = {-93, -62}, .b = {290, 298}}, {.a = {158, 168}, .b = {249, 252}}, {.a = {60, 62}, .b = {188, 184}}, {.a = {-4, -16}, .b = {-12, -30}}, {.a = {395, 389}, .b = {-11, -45}}, {.a = {31, -6}, .b = {318, 299}}, {.a = {-135, -204}, .b = {310, 287}}, {.a = {26, -34}, .b = {333, 309}}, {.a = {230, 188}, .b = {131, 70}}, {.a = {86, 11}, .b = {-104, -219}}, {.a = {304, 264}, .b = {249, 196}}, {.a = {296, 250}, .b = {321, 277}}, {.a = {177, 96}, .b = {309, 257}}, {.a = {270, 201}, .b = {20, -131}}, {.a = {102, -27}, .b = {-17, -191}}, {.a = {393, 356}, .b = {79, -78}}, {.a = {500, 500}, .b = {209, 89}}, {.a = {466, 451}, .b = {188, 45}}, {.a = {279, 173}, .b = {370, 304}}, {.a = {163, -10}, .b = {-43, -339}}, {.a = {247, 102}, .b = {150, -54}}, {.a = {84, -172}, .b = {223, 46}}, {.a = {113, -138}, .b = {454, 423}}, {.a = {192, -25}, .b = {99, -198}}, {.a = {361, 252}, .b = {500, 500}}, {.a = {193, -67}, .b = {339, 198}}, {.a = {382, 276}, .b = {40, -388}}, {.a = {336, 178}, .b = {300, 100}}, {.a = {195, -114}, .b = {360, 213}}, {.a = {145, -239}, .b = {398, 286}}, {.a = {210, -118}, .b = {422, 330}}, {.a = {221, -119}, .b = {444, 375}}, {.a = {245, -78}, .b = {315, 72}}, {.a = {253, -82}, .b = {308, 39}}, {.a = {304, 17}, .b = {330, 77}}, {.a = {268, -90}, .b = {296, -24}}, {.a = {142, -433}, .b = {433, 321}}, {.a = {260, -147}, .b = {286, -89}}, {.a = {461, 390}, .b = {463, 394}}, {.a = {177, -446}, .b = {237, -292}}, {.a = {318, -55}, .b = {334, -19}}, {.a = {500, 500}, .b = {328, -62}}, {.a = {417, 222}, .b = {425, 242}}, {.a = {284, -258}, .b = {297, -234}}, {.a = {344, -73}, .b = {397, 110}}, {.a = {306, -256}, .b = {459, 337}}, {.a = {437, 241}, .b = {352, -126}}, {.a = {444, 258}, .b = {344, -202}}, {.a = {459, 311}, .b = {456, 295}}, {.a = {378, -90}, .b = {356, -209}}, {.a = {387, -80}, .b = {375, -172}}, {.a = {415, 23}, .b = {448, 201}}, {.a = {500, 500}, .b = {401, -102}}, {.a = {428, 45}, .b = {474, 328}}, {.a = {371, -417}, .b = {428, -37}}, {.a = {439, 17}, .b = {452, 104}}, {.a = {439, -20}, .b = {433, -102}}, {.a = {414, -302}, .b = {408, -387}}, {.a = {452, -7}, .b = {467, 132}}, {.a = {451, -95}, .b = {446, -216}}, {.a = {479, 214}, .b = {477, 144}}, {.a = {459, -243}, .b = {468, -119}}, {.a = {482, 135}, .b = {465, -317}}, {.a = {481, 29}, .b = {475, -202}}, {.a = {481, -143}, .b = {486, -42}}, {.a = {491, 86}, .b = {495, 222}}, {.a = {496, 195}, .b = {498, 213}}, {.a = {501, -47}, .b = {506, -176}}, {.a = {506, 130}, .b = {516, -249}}, {.a = {507, 254}, .b = {512, 129}}, {.a = {531, -284}, .b = {525, -47}}, {.a = {543, -352}, .b = {519, 155}}, {.a = {536, -113}, .b = {515, 260}}, {.a = {506, 400}, .b = {529, 128}}, {.a = {546, -26}, .b = {572, -283}}, {.a = {564, -162}, .b = {555, -49}}, {.a = {556, -10}, .b = {557, 15}}, {.a = {543, 151}, .b = {516, 374}}, {.a = {608, -284}, .b = {616, -319}}, {.a = {605, -214}, .b = {587, -84}}, {.a = {559, 118}, .b = {577, 11}}, {.a = {620, -216}, .b = {608, -106}}, {.a = {569, 131}, .b = {623, -128}}, {.a = {685, -406}, .b = {551, 255}}, {.a = {681, -337}, .b = {591, 93}}, {.a = {689, -312}, .b = {646, -108}}, {.a = {718, -378}, .b = {629, -8}}, {.a = {644, -56}, .b = {550, 310}}, {.a = {696, -231}, .b = {652, -55}}, {.a = {624, 61}, .b = {609, 124}}, {.a = {641, 28}, .b = {749, -311}}, {.a = {737, -253}, .b = {577, 259}}, {.a = {703, -113}, .b = {660, 29}}, {.a = {548, 361}, .b = {575, 292}}, {.a = {731, -133}, .b = {582, 277}}, {.a = {851, -436}, .b = {691, 1}}, {.a = {786, -230}, .b = {616, 206}}, {.a = {692, 22}, .b = {709, -15}}, {.a = {664, 104}, .b = {655, 131}}, {.a = {588, 293}, .b = {700, 45}}, {.a = {629, 209}, .b = {630, 213}}, {.a = {503, 492}, .b = {625, 234}}, {.a = {719, 39}, .b = {835, -190}}, {.a = {578, 341}, .b = {652, 196}}, {.a = {728, 52}, .b = {778, -38}}, {.a = {948, -357}, .b = {843, -142}}, {.a = {790, -33}, .b = {826, -93}}, {.a = {717, 108}, .b = {773, 16}}, {.a = {916, -225}, .b = {586, 351}}, {.a = {648, 249}, .b = {811, -19}}, {.a = {782, 38}, .b = {592, 351}}, {.a = {622, 305}, .b = {817, 4}}, {.a = {861, -56}, .b = {638, 290}}, {.a = {710, 183}, .b = {818, 30}}, {.a = {779, 92}, .b = {867, -26}}, {.a = {509, 486}, .b = {663, 268}}, {.a = {744, 162}, .b = {1089, -307}}, {.a = {841, 37}, .b = {870, 6}}, {.a = {908, -36}, .b = {887, -1}}, {.a = {945, -66}, .b = {620, 348}}, {.a = {841, 75}, .b = {613, 360}}, {.a = {1109, -245}, .b = {668, 297}}, {.a = {858, 77}, .b = {809, 139}}, {.a = {959, -29}, .b = {847, 105}}, {.a = {702, 273}, .b = {810, 158}}, {.a = {574, 419}, .b = {578, 415}}, {.a = {736, 248}, .b = {691, 299}}, {.a = {754, 237}, .b = {792, 202}}, {.a = {995, 4}, .b = {1213, -200}}, {.a = {1118, -104}, .b = {720, 286}}, {.a = {819, 194}, .b = {930, 91}}, {.a = {909, 117}, .b = {713, 303}}, {.a = {668, 347}, .b = {943, 104}}, {.a = {848, 194}, .b = {897, 156}}, {.a = {829, 216}, .b = {680, 347}}, {.a = {631, 390}, .b = {873, 193}}, {.a = {954, 130}, .b = {1006, 91}}, {.a = {526, 478}, .b = {1125, 6}}, {.a = {1071, 55}, .b = {669, 369}}, {.a = {1116, 31}, .b = {1045, 92}}, {.a = {1183, -1}, .b = {690, 361}}, {.a = {974, 160}, .b = {1138, 46}}, {.a = {1132, 59}, .b = {798, 293}}, {.a = {871, 247}, .b = {1110, 88}}, {.a = {945, 203}, .b = {500, 500}}, {.a = {880, 253}, .b = {952, 209}}, {.a = {950, 216}, .b = {1225, 50}}, {.a = {1112, 124}, .b = {811, 311}}, {.a = {773, 336}, .b = {953, 232}}, {.a = {694, 387}, .b = {977, 226}}, {.a = {925, 259}, .b = {1232, 94}}, {.a = {932, 262}, .b = {895, 287}}, {.a = {804, 337}, .b = {967, 256}}, {.a = {781, 355}, .b = {992, 251}}, {.a = {802, 348}, .b = {623, 439}}, {.a = {767, 371}, .b = {1262, 139}}, {.a = {869, 328}, .b = {994, 275}}, {.a = {1200, 188}, .b = {950, 304}}, {.a = {714, 408}, .b = {747, 397}}, {.a = {1075, 264}, .b = {856, 355}}, {.a = {1273, 193}, .b = {822, 374}}, {.a = {934, 335}, .b = {809, 384}}, {.a = {1042, 300}, .b = {1003, 317}}, {.a = {885, 361}, .b = {977, 333}}, {.a = {1196, 261}, .b = {1264, 243}}, {.a = {854, 384}, .b = {1225, 269}}, {.a = {881, 382}, .b = {597, 470}}, {.a = {1021, 347}, .b = {1023, 351}}, {.a = {1439, 241}, .b = {635, 463}}, {.a = {660, 458}, .b = {811, 419}}, {.a = {1372, 282}, .b = {895, 405}}, {.a = {791, 433}, .b = {931, 404}}, {.a = {1350, 319}, .b = {1014, 395}}, {.a = {1481, 308}, .b = {693, 463}}, {.a = {1184, 374}, .b = {1233, 370}}, {.a = {1031, 411}, .b = {1089, 405}}, {.a = {707, 468}, .b = {814, 454}}, {.a = {967, 435}, .b = {955, 438}}, {.a = {1053, 429}, .b = {781, 465}}, {.a = {1205, 420}, .b = {639, 485}}, {.a = {645, 485}, .b = {1202, 437}}, {.a = {975, 460}, .b = {801, 477}}, {.a = {911, 471}, .b = {1151, 460}}, {.a = {1391, 449}, .b = {994, 474}}, {.a = {1201, 466}, .b = {500, 500}}, {.a = {978, 483}, .b = {1056, 483}}, {.a = {897, 490}, .b = {996, 489}}, {.a = {629, 498}, .b = {936, 497}}, {.a = {1204, 500}, .b = {838, 502}}, {.a = {500, 500}, .b = {948, 510}}, {.a = {993, 513}, .b = {1175, 523}}, {.a = {836, 513}, .b = {834, 517}}, {.a = {694, 511}, .b = {1006, 531}}, {.a = {500, 500}, .b = {1190, 549}}, {.a = {849, 526}, .b = {961, 537}}, {.a = {1203, 561}, .b = {1214, 566}}, {.a = {1245, 575}, .b = {1259, 583}}, {.a = {1024, 560}, .b = {945, 555}}
};

void	init_bsp(t_doom *doom)
{
	int count = sizeof(lines) / sizeof(t_line);
	t_arraylist *polygons = create_arraylist(count + 2);
	t_arraylist *vertices = create_arraylist(count);
	for (int i = 0; i < count; i++)
	{
		arraylist_appendm(&vertices, &(t_vec3){
			lines[i].a.x, 0, lines[i].a.y}, sizeof(t_vec3));
	}
	arraylist_append(&polygons, create_polygon(vertices, P_FLOOR));
	t_arraylist *vertices_ceil = create_arraylist(count);
	for (int i = 0; i < count; i++)
	{
		arraylist_appendm(&vertices_ceil, &(t_vec3){
			lines[i].a.x, 1, lines[i].a.y}, sizeof(t_vec3));
	}
	arraylist_append(&vertices_ceil, create_polygon(vertices, P_FLOOR));
	for (int i = 0; i < count; i++)
	{
		arraylist_append(&polygons, create_wall_polygon(lines[i], 0, 1));
	}
	t_node *n = create_node(polygons);
	build_node(n);
	printf("graph TD\n");
	print_node(n);
	doom->bsp = n;
}

int		main(void)
{
	t_doom doom = (t_doom) {
		.running = TRUE
	};
	init_bsp(&doom);
	init_sdl(&doom);
	if (!(doom.rendered_area = (uint8_t *)malloc(doom.screen.width)))
		return (-1);
	game_loop(&doom);
	sdl_quit(&doom);
	free_node(doom.bsp);
	return (0);
}

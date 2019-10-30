#pragma once

#include"chessPosition.h"


constexpr int chessPositionHashTable[2][7][10][9] =
{
{
		{
			{ 1906550789,1084895239,2056890377,1493176330,173570059,-102854645,-2034913267,1741117456,1402841104,            },
			{ 1984139281,-1754777579,357890072,1178370077,-1569708000,-972832734,-142620638,-1828429788,-1171333085,            },
			{ 858748966,1316683813,1054216228,1177767977,1635913769,1134970916,1433712684,89907244,1780521008,            },
			{ 1004156976,-234432459,1834289205,-1746876363,1332367419,1491716156,-233103297,-1118478273,1682401348,            },
			{ -448264121,-756369333,-1260994483,1816295502,-400670642,804968529,1019564114,-948680622,-991117228,            },
			{ -1671243691,-592717735,-2139826086,1779015769,1797412957,-1750851490,1562202207,916377697,1852268641,            },
			{ 546697315,-1198071708,-963389341,-377554843,1958942823,309696615,1723568233,1429129321,2144172138,            },
			{ -602003346,-1796374417,-1544019857,-128956306,-308000654,-1756180367,1661335667,-1577015182,1214922870,            },
			{ 140925050,1477238908,-1698000772,185325694,1661845631,849039487,-1230501759,-1079275391,1114456195,            },
			{ 629870725,1361883272,421220489,-1764183926,-1788399474,1923461264,-1466535788,620210325,20584598,            },
		},
		{
			{ 1930137751,-586207083,-1188308838,1245515931,-228845413,-1672042339,640714910,1813504154,-1757052767,            },
			{ 1477095589,-1092249433,-1777559384,-1466382167,-235659094,-1846736725,534704299,-1480376152,586723502,            },
			{ -573390673,-919138128,1090752690,2048319666,-1207990092,412928182,-2041003848,1759654072,221999295,            },
			{ 987930818,-1115094842,280580295,670316744,962154697,2128724170,422305996,-748050227,1645813966,            },
			{ -1333092144,1525539030,371151064,1157529817,1600088282,-1854732072,-1532485412,250792152,1121968350,            },
			{ -1941307168,448405728,306428128,580815073,2096693478,-10796824,-1864058644,-119545619,-858085137,            },
			{ 841396464,2043853043,1995940084,-1332086539,1532475638,1307681015,-1050943240,-718929671,-1700349701,            },
			{ -668374788,1706244350,80720126,-1559768833,1120891141,945821957,312076553,41572617,1504217355,            },
			{ 1759768844,-991684339,1849782540,-549201650,96655631,-548767471,665647378,1812189458,150798612,            },
			{ -1882427115,814117142,-776355561,-1054547686,-1185836773,2128210204,2098645277,335042848,-543690464,            },
		},
		{
			{ 1302214946,1988751651,1515241766,1593854252,-1840314062,1356433715,1771893045,-2059781834,-958523082,            },
			{ -379528907,-1922428615,1665415480,-300326597,546011449,1721243965,217006405,1418754376,-1991683768,            },
			{ 1152022856,963092808,-2043092660,-669286067,1315758414,-1071244977,-61879987,-1386901169,-1308962478,            },
			{ -1988048557,777218388,559634777,1792440667,1158541660,-899473059,-795131555,1513253216,1776163169,            },
			{ 2004189537,-1639630493,1798336867,-1163310747,-2099320478,737532260,-1371416216,-228644503,-516449943,            },
			{ 631141,-412075668,856402285,-590665359,-1299291789,-906718860,-1114777229,1102102908,455799165,            },
			{ 272439678,910180738,-1925215869,2083213700,-1822426748,1698558344,1899377033,-381386356,-837539443,            },
			{ -1856022132,-1593556592,-363175535,-1768232554,1088913815,1504258455,764524953,-776750696,-593032805,            },
			{ -1717300835,713738653,1315692959,-147453534,1707250088,-1938007638,1170688429,971006381,-326164049,            },
			{ 411539888,-2144155214,-2143966797,1599185332,-212002381,1072173492,-370208327,-857071174,-1734364741,            },
		},
		{
			{ -900814403,-862391871,-701521467,1681504709,2099798471,-1836842553,2005156299,-872789554,-1246148145,            },
			{ -217146929,2058551762,-1250002477,1514238420,315789781,1857169881,-1610100261,-914292259,1687497185,            },
			{ -394896926,1597221346,740731364,-589579803,-1481160221,160047591,-16191000,-2052107804,455639530,            },
			{ -1791702549,1234170341,2019047908,-1475520019,1803846121,-1206421007,205275639,-1097115141,-807466500,            },
			{ 358885885,-329074177,404845060,-422030843,1128749574,-1019698684,-93242876,1851664902,-401827322,            },
			{ 606554641,830796305,1214593553,-469693935,-1432595947,-1740223979,-604155371,303000088,-1996010987,            },
			{ -60757482,274055702,-261254626,549188128,189684258,-1187739102,-77880796,366678563,-1180861913,            },
			{ -1961448919,-1018213846,-723842517,-374910419,1503898157,-1564841427,1411183152,1438517811,1404174899,            },
			{ 1027680821,-1318729160,1434026554,1471918651,-822166980,1858923068,918387262,-1896361409,1313000000,            },
			{ 1837595201,1814635071,1203677756,1563886142,-246255035,428884550,16026182,-982535610,-2103428538,            },
		},
		{
			{ 1278038603,-61124021,108655182,-911879601,1590073938,389038675,1007938132,9773656,-1777962406,            },
			{ 666391133,-1524989346,2015713887,-75816353,1725483617,1145453153,1882690147,749273695,-758158755,            },
			{ 1222486630,-460107160,1016017513,-1094602134,508119657,830878315,-1370174869,439413354,261585512,            },
			{ -1243702601,438674033,910905969,-437630348,1871643256,1735441016,872727160,-275158408,-1632593285,            },
			{ -1305548167,-1186239877,-65451392,-1038564731,1710224007,-1660474745,1216164488,-952440182,1017326218,            },
			{ 2026394248,479089293,1226513038,-1893223800,-159343984,2121280146,1290330774,269359767,1654948502,            },
			{ -176270695,1289431706,-880682341,-74969795,2069797533,1680732828,800342688,1986947744,-426974559,            },
			{ -1845484891,952623782,1086491302,1171303081,-1665633617,1682641585,1930091188,-1198642507,1940417205,            },
			{ -1714033993,128420536,-1217758539,-1509076294,1937451490,-2059124044,-1615277382,-360238402,-583730498,            },
			{ -232377664,74709696,-1046449470,-1886438723,-1251978556,839975621,1534980805,1738384067,480418506,            },
		},
		{
			{ -1062176053,13304526,-1667640625,-1589822769,1891554002,392170197,-710030626,807285472,-1745118492,            },
			{ -1087087899,1120613096,1281331944,-1880993048,54403169,896185071,1552640755,1031756534,-1451547912,            },
			{ -648203524,-1984562436,-1908665602,769012479,-888286464,2120018689,-1928754432,24908547,-137442556,            },
			{ -569050363,-2012763386,-79359225,-195030264,-1290509559,-2091465980,-762565877,-680256756,1378749196,            },
			{ 1006793487,-2036653297,-745739504,-2021170414,334570265,146277145,-837678308,173454109,-842990818,            },
			{ -1687948513,-1203176672,-1321276642,1925999398,1138291495,-794633433,-716559575,-522290387,-909587667,            },
			{ 831200047,-1769471183,180794161,-1172065485,1605595955,-255145164,1869241142,-1185453257,-1833993418,            },
			{ -126858441,-78570693,-1401580740,-1199158467,1128100670,2144328510,884833086,-1216756927,-306877631,            },
			{ 499800899,6273859,1860590405,854125380,1372377929,822512458,-2051841197,-68981932,512363349,            },
			{ -1470485676, 1015923543, -732085417, -1691751593, -1437007016, -585557160, 784065373, -21902497, -964379807, },
		},
		{
			{ -399912095,1552348002,881443686,1158996839,423345002,1866834795,1998281579,848694123,563915632,            },
			{ 583408499,2041643892,-63272075,-210246796,-424508553,-1956281476,-94061486,-1929317506,-1143362688,            },
			{ 1982630793,591924106,1952351116,-970585204,1765530513,-1568246889,-1121782889,-1581753449,-2022681704,            },
			{ 1196888988,-1041298530,2055908254,475280288,918107043,819631015,-593296469,-188853332,-1473313876,            },
			{ -1287980114,1422375852,1300192171,1888809905,1458146226,-1744751692,-1236810827,-247442505,-1074492487,            },
			{ -792243269,1657512892,1892674491,321338304,957303745,-240192574,506645443,35959748,-847383613,            },
			{ -1310409785,217895879,-1931891767,-1156895798,2005388232,1281283021,978305998,45284305,-1765047340,            },
			{ -922399788,1560140758,-1045165098,-1243583528,-1470483495,255200216,823495643,673352668,323574747,            },
			{ -725242922,-307412001,1651217375,411790305,-896406559,315007970,-1046117404,-1938166811,967986152,            },
			{ -867062806,-831626262,1511056366,470076399,-449137680,-34176017,-494752780,-481649674,662733817,            },
		},
	},
	{
		{
			{ 1230302202,-760462342,-1969421315,744143870,634960893,-1180494847,1475714051,-1838580732,1651500036,            },
			{ 1857551367,556870670,1824459791,1269935119,-1749904370,-708547566,29529107,1048685588,636910613,            },
			{ 115809302,2078979096,-1569235943,2129292314,-855182309,1865536543,-1227432928,-294898655,-852667358,            },
			{ 1619008547,-1364261852,1193096229,1173480482,-1246546902,-1254884308,1728019501,1007379502,-219337681,            },
			{ -1698628562,1005052979,946994230,-1251570633,379698231,-344099778,-76450751,-835529663,-1866318781,            },
			{ -1880112054,671675467,1647819852,1220058191,958733391,673037392,1008622674,850566227,-415151020,            },
			{ -1003400107,-149472173,369103959,-561859496,809350232,-1888474023,-1256737703,273271891,-1317264291,            },
			{ 980974681,114658399,241290334,-1484794783,-1196176290,651818083,1453638755,432237669,-960623514,            },
			{ -212114328,1482042475,2131840107,-1366958996,-1804745617,1516541041,-1813175182,1344722035,1856588915,            },
			{ 346670195,107943030,-1781205902,1435491448,760992889,1963967610,1240792187,-858127241,45411445,            },
		},
		{
			{ 269015739,-487463809,1116195967,-234513279,1820900479,123239556,-1154526076,-815254396,-1065530233,            },
			{ -1942643577,-1676452725,1269101707,-1039868436,1256942734,1735001230,393520271,-1105576815,1052286097,            },
			{ 1284682899,1435182225,1301681301,1749531798,1770405015,-1013635944,1457177753,1065387162,-1629829997,            },
			{ -1882659359,556676253,-903885666,1638843553,-1506855775,510655651,-1063768,-844532568,1981586602,            },
			{ 845532328,-1315928916,916434092,-1114520402,127571965,-1079321422,-1025680205,-1813171023,1252371633,            },
			{ -227963725,-95157065,-1491374921,-1178721094,-1876931397,-1603158850,76367042,-62491453,-61809466,            },
			{ 1614486728,-1025624887,1427504330,1480371403,1865696457,775924938,1240536270,1489835215,-665649969,            },
			{ 1164340433,531569874,80563411,-1323985711,1130155214,1222290649,-885746468,1119757534,606506207,            },
			{ -865225500,1666053351,-1537944344,2085610727,351413482,-23032597,217906412,854729965,64423145,            },
			{ -66417425,1181940970,-345254672,197596397,739308787,-764709645,728339695,-1038854921,-609340168,            },
		},
		{
			{ 1595321593,-1126236934,-1620445957,220017916,-293137155,576906490,-529963781,-1306313472,-1217692414,            },
			{ 920128771,408401157,560194821,-1485804280,1127914764,-1053637363,-800541426,-1648923378,-2123215599,            },
			{ -2129466094,1117377810,-1166482150,-646832868,77235484,-1345948386,-876952289,-340288224,-614150882,            },
			{ -898966224,-917684943,-1865169616,-1394535117,186963249,2073859380,1104909621,72672567,205978938,            },
			{ 1956144445,-993147586,1054303550,-1132591808,169895232,-758440638,-818014909,2103166271,-819550907,            },
			{ 1155177798,248894790,-1112001204,-1074113203,-1594890928,2052693329,1041089875,1812655444,-321219243,            },
			{ -1204032167,-1690215078,1244325211,-805294754,-959742624,-522062759,1396671842,1422710115,-1583771,            },
			{ -2001715863,1015498089,1963984235,-1343290005,1665854829,132511085,471410032,1362027890,-1086339725,            },
			{ 1089176949,-1493697161,-2135562888,1676221818,-686346881,1955628423,-741538425,370201994,826674572,            },
			{ -400472691,444632466,-1514227721,1531231639,1990391193,280194459,-1541605985,26058146,1410444707,            },
		},
		{
			{ 366673315,-1031897689,921748904,776043944,-353878612,1093803438,1261866414,-1375584849,-940442194,            },
			{ 1372407219,-1128208972,1735101877,-604492363,922621367,602408377,728804794,-2122748487,-1507697220,            },
			{ -18065987,-257647171,-1826185793,-562033214,804389564,-466334265,-461470265,161353162,-30304815,            },
			{ -2114486830,-874418728,218607065,-2109196837,-1106981411,-1394192931,-159889953,1000062969,-1362285087,            },
			{ -1489732126,533194211,246861284,1014330850,25869795,1656225252,1685063144,1230570984,1589288426,            },
			{ 1108487657,160345580,808115692,1956646377,109477359,888540656,-2034024975,-710711831,1874181619,            },
			{ 2137576944,-40122891,-1033845265,-1708538382,1092732408,1195453945,1893742067,1277064701,-1937760770,            },
			{ -1758540288,1236813312,40156677,1486007816,1278838280,-669917686,-170164728,1871824396,-512524783,            },
			{ 2016396820,-1896327658,-1942981097,-151550441,1277285915,-2077217251,1331912224,1546688032,-2104150493,            },
			{ 593868325,-1331397078,-442245590,796225068,-225868245,417652270,-1908394445,-2144829900,1109466676,            },
		},
		{
			{ 1594398263,-1170358727,-631824838,-41152967,-371526080,1583771204,-1174243772,-787591607,455710282,            },
			{ -541798836,1340716623,1818846800,-2036277679,239015506,-2125459884,2066443861,-2009657771,68539991,            },
			{ -504478116,1001268830,159954527,-1093661086,-903874972,1443448421,363318886,1075381866,-487070099,            },
			{ -389522,-535970193,1432147565,-635349391,-628740494,1552219769,496531066,696176250,447587964,            },
			{ 1684940419,787164803,-334047611,-251263354,-15382907,-86016372,1657790094,-709216625,1099357841,            },
			{ 182408850,-1834948973,1507907218,-2134157674,922185368,1275279000,952235674,1991007899,1721231004,            },
			{ 619935389,-718217570,228128412,-624327008,692899489,-1732491614,405450402,-451180891,-933247323,            },
			{ 1939322535,-1821792602,-1482369371,-1010807124,970962604,2039887534,-148795729,1489411756,-916803919,            },
			{ -199156046,1025611438,619445942,-1554073925,-821094722,-2094510402,867256000,1989142209,509210308,            },
			{ 138737349,1589176005,-1602560314,-1574920503,-582125878,2011969225,-1609769268,-2046787892,-1986341171,            },
		},
		{
			{ -1936779568,856907475,-1016656172,-1192632619,1569365718,1691285208,-1565198631,319958746,-1017223460,            },
			{ 581334749,1695993566,1060253407,587405024,84874972,1967789170,-1093450011,1875930859,-1274251541,            },
			{ -181043473,1360977648,831350513,-1393357071,-1739464973,207335155,1712664306,-312498445,-1110268169,            },
			{ 1909442290,1919000306,-189526278,-202940676,1297766141,-317364483,-181045501,-1570920700,-1109922044,            },
			{ 494614279,-1939540217,1480937226,-1508610294,1382508300,375754509,700043021,2085885711,1325045520,            },
			{ 1274926864,315162387,97502996,-1200711913,701927192,1228603160,-1392535784,-1478852840,-1040861412,            },
			{ 1172866839,638562075,-1937729769,-618531038,-1808861405,-363239645,1250598691,1597425446,1551046435,            },
			{ -2110228694,67018539,-1202282708,-537710800,-1393193166,1336710966,2070417206,-1099452615,1848821563,            },
			{ 1319704382,567854911,1774073663,-202172606,2111678275,1105133384,2035971913,769734472,1197801292,            },
			{ 1563725645,-1520167090,1290649422,-485966000,-1611714731,-1497587883,-91900072,2045292377,-234375336,            },
		},
		{
			{ -1989992611,1522343773,269270877,-1366016161,-1857960095,683214686,1763794787,1428131682,-1138804042,            },
			{ -963903642,-359444641,-697620574,-497051797,-1410482323,690229102,-2133360786,-1003473043,-2075422860,            },
			{ -1499338884,1619527552,490135426,1004781443,2145175434,1972567947,1565566861,1795004303,1149573009,            },
			{ -672254060,-2094549098,882374648,-1789462630,-581298275,-1386182753,829740960,1988079522,-1806493789,            },
			{ -636375132,794208165,-990517338,-1256409177,-118399064,-1775437914,-1772621914,-618795101,-2018248787,            },
			{ -1887823954,948834227,-514957387,1840396214,-1831159880,-1807020102,-102242373,844363706,-683968577,            },
			{ -28385345,-1207779390,-269793340,-22526010,-708474937,-1644582968,340629449,2007060426,567345099,            },
			{ 1760839627,666740682,-370726962,-15474737,-1386543153,1756968913,-1642858542,-291581992,1460897755,            },
			{ -1228519460,690763743,1520072677,337813478,1823850471,450222058,-987916304,-1166821391,504496113,            },
			{ -711448591,1673185268,324667381,-1396199435,414488567,1832534004,1800071161,1625288699,-997128195,            },
		},
	}
};

int getChessPositionHash(ChessPosition* cp)
{
	auto& chess = cp->chess;
	if (chess->type == None)
	{
		return 0;
	}
	return chessPositionHashTable[chess->country][chess->type - 1][cp->y][cp->x];
}

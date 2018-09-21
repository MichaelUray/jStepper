/*
 * jsmath.h
 * 
 * misc math macros
 */

// misc math constants
#define LONG_ERROR -2147483648
#ifndef PI
  #define PI 3.14159265358979323846
#endif
#define ONE_MILLION 1000000.0

// Macros to contrain values
#define NOTLESS(v,n) do{ if (v < n) v = n; }while(0)
#define NOTMORE(v,n) do{ if (v > n) v = n; }while(0)

// Macros for chars
#define IS_WITHIN(V,L,H) ((V) >= (L) && (V) <= (H))
#define IS_NUMERIC(a) IS_WITHIN(a, '0', '9')     // test for numeric vals 0-9
#define IS_ALPHA_UC(a) IS_WITHIN(a, 'A', 'Z')    // test for upper case alpha chars
#define IS_ALPHA_LC(a) IS_WITHIN(a, 'a', 'z')    // test for lower case alpha chars
#define IS_DECIMAL(a) (IS_NUMERIC(a) || a == '.')
#define IS_NUMERIC_SIGNED(a) (IS_NUMERIC(a) || (a) == '-' || (a) == '+')
#define IS_DECIMAL_SIGNED(a) (IS_DECIMAL(a) || (a) == '-' || (a) == '+')

// macros for arrays
#define NUM_ELEMENTS(a) (sizeof(a) / sizeof(*a))		// returns the number of elements in the array

/*
 * Embedded systems require fast solutions to avoid computationally expensive
 * math. This solution is a bit unorthidox but does the trick.
 *
 * This lookup scheme converts an initial start interval (_c0) and a step
 * number (n) into a correct time interval for the subsequent step (_cn). The 
 * function is equivalent to:
 * _cn = _cn - (_c0 * (sqrt(n+1) - sqrt(n))) 
 * Lookup[n] = _cn / _c0 * 65536
 * 
 * The lookup scheme performs the above calc without the need for
 * square root or division and is resolved using a single integer
 * multiply and the trick of punning.
 *
 */

#ifndef LOOKUP_H
#define LOOKUP_H

#define LOOKUP_TBL_SIZE 512

const uint16_t accel_lookup_table[LOOKUP_TBL_SIZE+1] PROGMEM = {
   65535, 39319, 30578, 25870, 22824, 20648, 18994, 17682, 16608, 15708, 
   14938, 14272, 13684, 13162, 12696, 12274, 11892, 11542, 11220, 10924, 
   10650, 10394, 10156, 9932, 9722, 9524, 9338, 9162, 8994, 8836, 
   8684, 8540, 8402, 8270, 8144, 8024, 7908, 7796, 7690, 7588, 
   7488, 7392, 7300, 7210, 7124, 7040, 6958, 6880, 6804, 6728, 
   6654, 6582, 6512, 6444, 6378, 6314, 6250, 6188, 6128, 6070, 
   6012, 5956, 5902, 5848, 5796, 5744, 5694, 5644, 5596, 5548, 
   5502, 5456, 5412, 5368, 5324, 5282, 5240, 5200, 5160, 5120, 
   5082, 5044, 5006, 4968, 4932, 4896, 4860, 4826, 4792, 4758, 
   4724, 4692, 4660, 4628, 4596, 4564, 4534, 4504, 4474, 4444, 
   4414, 4386, 4358, 4330, 4302, 4274, 4246, 4220, 4194, 4168, 
   4142, 4116, 4090, 4064, 4040, 4016, 3992, 3968, 3944, 3920, 
   3896, 3872, 3850, 3828, 3806, 3784, 3762, 3740, 3718, 3696, 
   3674, 3654, 3634, 3614, 3594, 3574, 3554, 3534, 3514, 3494, 
   3474, 3454, 3434, 3416, 3398, 3380, 3362, 3342, 3322, 3302, 
   3282, 3262, 3242, 3222, 3202, 3182, 3162, 3142, 3124, 3106, 
   3088, 3070, 3052, 3034, 3016, 2998, 2980, 2962, 2944, 2926, 
   2908, 2890, 2872, 2854, 2836, 2818, 2802, 2786, 2770, 2754, 
   2738, 2722, 2706, 2690, 2674, 2658, 2642, 2626, 2610, 2596, 
   2582, 2568, 2554, 2540, 2526, 2512, 2498, 2484, 2470, 2456, 
   2442, 2428, 2414, 2402, 2390, 2378, 2366, 2354, 2342, 2330, 
   2318, 2306, 2294, 2282, 2270, 2258, 2246, 2234, 2222, 2210, 
   2198, 2186, 2174, 2162, 2150, 2138, 2126, 2114, 2102, 2090, 
   2078, 2066, 2054, 2042, 2030, 2018, 2006, 1994, 1982, 1970, 
   1958, 1946, 1934, 1924, 1914, 1904, 1894, 1884, 1874, 1864, 
   1854, 1844, 1834, 1824, 1814, 1804, 1794, 1784, 1774, 1764, 
   1754, 1744, 1734, 1724, 1714, 1704, 1694, 1684, 1674, 1664, 
   1654, 1644, 1634, 1624, 1614, 1604, 1594, 1584, 1574, 1564, 
   1554, 1544, 1534, 1524, 1514, 1504, 1494, 1484, 1474, 1464, 
   1454, 1444, 1434, 1424, 1414, 1404, 1394, 1384, 1374, 1364, 
   1354, 1344, 1334, 1324, 1314, 1304, 1294, 1284, 1274, 1264, 
   1254, 1244, 1236, 1228, 1220, 1212, 1204, 1196, 1188, 1180, 
   1172, 1164, 1156, 1148, 1140, 1132, 1124, 1116, 1108, 1100, 
   1092, 1084, 1076, 1068, 1060, 1052, 1044, 1036, 1028, 1020, 
   1012, 1004, 996, 988, 980, 972, 964, 956, 948, 940, 
   932, 926, 920, 914, 908, 902, 896, 890, 884, 878, 
   872, 866, 860, 854, 848, 842, 836, 830, 824, 818, 
   812, 806, 800, 794, 788, 782, 776, 770, 764, 758, 
   752, 746, 740, 734, 728, 722, 716, 710, 704, 698, 
   692, 686, 680, 674, 668, 662, 656, 650, 644, 638, 
   632, 626, 620, 614, 608, 602, 596, 590, 584, 578, 
   572, 566, 560, 554, 548, 542, 536, 530, 524, 518, 
   512, 506, 500, 494, 488, 482, 476, 470, 464, 458, 
   452, 446, 440, 434, 428, 422, 416, 410, 404, 398, 
   392, 386, 380, 374, 368, 364, 360, 356, 352, 348, 
   344, 340, 336, 332, 328, 324, 320, 316, 312, 308, 
   304, 300, 296, 292, 288, 284, 280, 276, 272, 268, 
   264, 260, 256, 252, 248, 244, 240, 236, 232, 228, 
   224, 220, 216, 212, 208, 204, 200, 196, 192, 188, 
   184, 180, 176, 172, 168, 164, 160, 156, 152, 148, 
   144, 140, 136, 132, 128, 124, 120, 116, 112, 108, 
   104, 102, 100, 

};



#endif  // LOOKUP_H

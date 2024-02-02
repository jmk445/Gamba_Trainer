#ifndef MFCC_H
#define MFCC_H

#include <math.h>
#include <float.h>
#include <string.h>
#include "esp_log.h"
#include "esp_dsp.h"

#define WINDOW_FUNC {0.000000, 0.000024, 0.000096, 0.000217, 0.000385, 0.000602, 0.000867, 0.001180, 0.001541, 0.001950, 0.002408, 0.002913, 0.003466, 0.004067, 0.004715, 0.005412, 0.006156, 0.006948, 0.007787, 0.008673, 0.009607, 0.010589, 0.011617, 0.012693, 0.013815, 0.014984, 0.016200, 0.017463, 0.018772, 0.020128, 0.021530, 0.022978, 0.024472, 0.026012, 0.027597, 0.029228, 0.030904, 0.032626, 0.034393, 0.036204, 0.038060, 0.039961, 0.041906, 0.043895, 0.045928, 0.048005, 0.050126, 0.052290, 0.054497, 0.056747, 0.059039, 0.061375, 0.063752, 0.066171, 0.068633, 0.071136, 0.073680, 0.076265, 0.078891, 0.081558, 0.084265, 0.087012, 0.089799, 0.092626, 0.095492, 0.098396, 0.101340, 0.104322, 0.107342, 0.110399, 0.113495, 0.116627, 0.119797, 0.123003, 0.126246, 0.129524, 0.132839, 0.136188, 0.139573, 0.142993, 0.146447, 0.149935, 0.153456, 0.157011, 0.160600, 0.164221, 0.167874, 0.171559, 0.175276, 0.179024, 0.182803, 0.186613, 0.190453, 0.194323, 0.198222, 0.202150, 0.206107, 0.210093, 0.214106, 0.218147, 0.222215, 0.226310, 0.230431, 0.234578, 0.238751, 0.242949, 0.247171, 0.251418, 0.255689, 0.259984, 0.264302, 0.268642, 0.273005, 0.277389, 0.281795, 0.286222, 0.290670, 0.295138, 0.299626, 0.304132, 0.308658, 0.313203, 0.317765, 0.322345, 0.326941, 0.331555, 0.336185, 0.340831, 0.345491, 0.350167, 0.354858, 0.359562, 0.364280, 0.369011, 0.373754, 0.378510, 0.383277, 0.388056, 0.392845, 0.397645, 0.402455, 0.407274, 0.412102, 0.416938, 0.421783, 0.426635, 0.431494, 0.436359, 0.441231, 0.446109, 0.450991, 0.455879, 0.460770, 0.465666, 0.470565, 0.475466, 0.480370, 0.485276, 0.490183, 0.495091, 0.500000, 0.504909, 0.509817, 0.514724, 0.519630, 0.524534, 0.529435, 0.534334, 0.539230, 0.544121, 0.549009, 0.553891, 0.558769, 0.563640, 0.568506, 0.573365, 0.578217, 0.583062, 0.587898, 0.592726, 0.597545, 0.602355, 0.607155, 0.611944, 0.616723, 0.621490, 0.626246, 0.630989, 0.635720, 0.640438, 0.645142, 0.649833, 0.654508, 0.659169, 0.663815, 0.668445, 0.673059, 0.677655, 0.682235, 0.686797, 0.691342, 0.695867, 0.700374, 0.704862, 0.709330, 0.713778, 0.718205, 0.722611, 0.726995, 0.731358, 0.735698, 0.740016, 0.744311, 0.748582, 0.752829, 0.757051, 0.761249, 0.765422, 0.769569, 0.773690, 0.777785, 0.781853, 0.785894, 0.789907, 0.793893, 0.797850, 0.801778, 0.805677, 0.809547, 0.813387, 0.817197, 0.820976, 0.824724, 0.828441, 0.832126, 0.835779, 0.839400, 0.842989, 0.846544, 0.850065, 0.853553, 0.857007, 0.860427, 0.863812, 0.867161, 0.870476, 0.873754, 0.876997, 0.880203, 0.883373, 0.886505, 0.889601, 0.892658, 0.895678, 0.898660, 0.901604, 0.904508, 0.907374, 0.910201, 0.912988, 0.915735, 0.918442, 0.921109, 0.923735, 0.926320, 0.928864, 0.931367, 0.933829, 0.936248, 0.938625, 0.940961, 0.943253, 0.945503, 0.947710, 0.949874, 0.951995, 0.954072, 0.956105, 0.958094, 0.960039, 0.961940, 0.963796, 0.965607, 0.967374, 0.969096, 0.970772, 0.972403, 0.973988, 0.975528, 0.977022, 0.978470, 0.979872, 0.981228, 0.982537, 0.983800, 0.985016, 0.986185, 0.987307, 0.988383, 0.989411, 0.990393, 0.991327, 0.992213, 0.993052, 0.993844, 0.994588, 0.995285, 0.995933, 0.996534, 0.997087, 0.997592, 0.998049, 0.998459, 0.998820, 0.999133, 0.999398, 0.999615, 0.999783, 0.999904, 0.999976, 1.000000, 0.999976, 0.999904, 0.999783, 0.999615, 0.999398, 0.999133, 0.998820, 0.998459, 0.998049, 0.997592, 0.997087, 0.996534, 0.995933, 0.995285, 0.994588, 0.993844, 0.993052, 0.992213, 0.991327, 0.990393, 0.989411, 0.988383, 0.987307, 0.986185, 0.985016, 0.983800, 0.982537, 0.981228, 0.979872, 0.978470, 0.977022, 0.975528, 0.973988, 0.972403, 0.970772, 0.969096, 0.967374, 0.965607, 0.963796, 0.961940, 0.960039, 0.958094, 0.956105, 0.954072, 0.951995, 0.949874, 0.947710, 0.945503, 0.943253, 0.940961, 0.938625, 0.936248, 0.933829, 0.931367, 0.928864, 0.926320, 0.923735, 0.921109, 0.918442, 0.915735, 0.912988, 0.910201, 0.907374, 0.904508, 0.901604, 0.898660, 0.895678, 0.892658, 0.889601, 0.886505, 0.883373, 0.880203, 0.876997, 0.873754, 0.870476, 0.867161, 0.863812, 0.860427, 0.857007, 0.853553, 0.850065, 0.846544, 0.842989, 0.839400, 0.835779, 0.832126, 0.828441, 0.824724, 0.820976, 0.817197, 0.813387, 0.809547, 0.805677, 0.801778, 0.797850, 0.793893, 0.789907, 0.785894, 0.781853, 0.777785, 0.773690, 0.769569, 0.765422, 0.761249, 0.757051, 0.752829, 0.748582, 0.744311, 0.740016, 0.735698, 0.731358, 0.726995, 0.722611, 0.718205, 0.713778, 0.709330, 0.704862, 0.700374, 0.695867, 0.691342, 0.686797, 0.682235, 0.677655, 0.673059, 0.668445, 0.663815, 0.659169, 0.654508, 0.649833, 0.645142, 0.640438, 0.635720, 0.630989, 0.626246, 0.621490, 0.616723, 0.611944, 0.607155, 0.602355, 0.597545, 0.592726, 0.587898, 0.583062, 0.578217, 0.573365, 0.568506, 0.563640, 0.558769, 0.553891, 0.549009, 0.544121, 0.539230, 0.534334, 0.529435, 0.524534, 0.519630, 0.514724, 0.509817, 0.504909, 0.500000, 0.495091, 0.490183, 0.485276, 0.480370, 0.475466, 0.470565, 0.465666, 0.460770, 0.455879, 0.450991, 0.446109, 0.441231, 0.436359, 0.431494, 0.426635, 0.421783, 0.416938, 0.412102, 0.407274, 0.402455, 0.397645, 0.392845, 0.388056, 0.383277, 0.378510, 0.373754, 0.369011, 0.364280, 0.359562, 0.354858, 0.350167, 0.345491, 0.340831, 0.336185, 0.331555, 0.326941, 0.322345, 0.317765, 0.313203, 0.308658, 0.304132, 0.299626, 0.295138, 0.290670, 0.286222, 0.281795, 0.277389, 0.273005, 0.268642, 0.264302, 0.259984, 0.255689, 0.251418, 0.247171, 0.242949, 0.238751, 0.234578, 0.230431, 0.226310, 0.222215, 0.218147, 0.214106, 0.210093, 0.206107, 0.202150, 0.198222, 0.194323, 0.190453, 0.186613, 0.182803, 0.179024, 0.175276, 0.171559, 0.167874, 0.164221, 0.160600, 0.157011, 0.153456, 0.149935, 0.146447, 0.142993, 0.139573, 0.136188, 0.132839, 0.129524, 0.126246, 0.123003, 0.119797, 0.116627, 0.113495, 0.110399, 0.107342, 0.104322, 0.101340, 0.098396, 0.095492, 0.092626, 0.089799, 0.087012, 0.084265, 0.081558, 0.078891, 0.076265, 0.073680, 0.071136, 0.068633, 0.066171, 0.063752, 0.061375, 0.059039, 0.056747, 0.054497, 0.052290, 0.050126, 0.048005, 0.045928, 0.043895, 0.041906, 0.039961, 0.038060, 0.036204, 0.034393, 0.032626, 0.030904, 0.029228, 0.027597, 0.026012, 0.024472, 0.022978, 0.021530, 0.020128, 0.018772, 0.017463, 0.016200, 0.014984, 0.013815, 0.012693, 0.011617, 0.010589, 0.009607, 0.008673, 0.007787, 0.006948, 0.006156, 0.005412, 0.004715, 0.004067, 0.003466, 0.002913, 0.002408, 0.001950, 0.001541, 0.001180, 0.000867, 0.000602, 0.000385, 0.000217, 0.000096, 0.000024, }

#define TWIDDLECOEF_RFFT_1024 { 0.000000000f,  1.000000000f, 0.006135885f,  0.999981175f, 0.012271538f,  0.999924702f, 0.018406730f,  0.999830582f, 0.024541229f,  0.999698819f, 0.030674803f,  0.999529418f, 0.036807223f,  0.999322385f, 0.042938257f,  0.999077728f, 0.049067674f,  0.998795456f, 0.055195244f,  0.998475581f, 0.061320736f,  0.998118113f, 0.067443920f,  0.997723067f, 0.073564564f,  0.997290457f, 0.079682438f,  0.996820299f, 0.085797312f,  0.996312612f, 0.091908956f,  0.995767414f, 0.098017140f,  0.995184727f, 0.104121634f,  0.994564571f, 0.110222207f,  0.993906970f, 0.116318631f,  0.993211949f, 0.122410675f,  0.992479535f, 0.128498111f,  0.991709754f, 0.134580709f,  0.990902635f, 0.140658239f,  0.990058210f, 0.146730474f,  0.989176510f, 0.152797185f,  0.988257568f, 0.158858143f,  0.987301418f, 0.164913120f,  0.986308097f, 0.170961889f,  0.985277642f, 0.177004220f,  0.984210092f, 0.183039888f,  0.983105487f, 0.189068664f,  0.981963869f, 0.195090322f,  0.980785280f, 0.201104635f,  0.979569766f, 0.207111376f,  0.978317371f, 0.213110320f,  0.977028143f, 0.219101240f,  0.975702130f, 0.225083911f,  0.974339383f, 0.231058108f,  0.972939952f, 0.237023606f,  0.971503891f, 0.242980180f,  0.970031253f, 0.248927606f,  0.968522094f, 0.254865660f,  0.966976471f, 0.260794118f,  0.965394442f, 0.266712757f,  0.963776066f, 0.272621355f,  0.962121404f, 0.278519689f,  0.960430519f, 0.284407537f,  0.958703475f, 0.290284677f,  0.956940336f, 0.296150888f,  0.955141168f, 0.302005949f,  0.953306040f, 0.307849640f,  0.951435021f, 0.313681740f,  0.949528181f, 0.319502031f,  0.947585591f, 0.325310292f,  0.945607325f, 0.331106306f,  0.943593458f, 0.336889853f,  0.941544065f, 0.342660717f,  0.939459224f, 0.348418680f,  0.937339012f, 0.354163525f,  0.935183510f, 0.359895037f,  0.932992799f, 0.365612998f,  0.930766961f, 0.371317194f,  0.928506080f, 0.377007410f,  0.926210242f, 0.382683432f,  0.923879533f, 0.388345047f,  0.921514039f, 0.393992040f,  0.919113852f, 0.399624200f,  0.916679060f, 0.405241314f,  0.914209756f, 0.410843171f,  0.911706032f, 0.416429560f,  0.909167983f, 0.422000271f,  0.906595705f, 0.427555093f,  0.903989293f, 0.433093819f,  0.901348847f, 0.438616239f,  0.898674466f, 0.444122145f,  0.895966250f, 0.449611330f,  0.893224301f, 0.455083587f,  0.890448723f, 0.460538711f,  0.887639620f, 0.465976496f,  0.884797098f, 0.471396737f,  0.881921264f, 0.476799230f,  0.879012226f, 0.482183772f,  0.876070094f, 0.487550160f,  0.873094978f, 0.492898192f,  0.870086991f, 0.498227667f,  0.867046246f, 0.503538384f,  0.863972856f, 0.508830143f,  0.860866939f, 0.514102744f,  0.857728610f, 0.519355990f,  0.854557988f, 0.524589683f,  0.851355193f, 0.529803625f,  0.848120345f, 0.534997620f,  0.844853565f, 0.540171473f,  0.841554977f, 0.545324988f,  0.838224706f, 0.550457973f,  0.834862875f, 0.555570233f,  0.831469612f, 0.560661576f,  0.828045045f, 0.565731811f,  0.824589303f, 0.570780746f,  0.821102515f, 0.575808191f,  0.817584813f, 0.580813958f,  0.814036330f, 0.585797857f,  0.810457198f, 0.590759702f,  0.806847554f, 0.595699304f,  0.803207531f, 0.600616479f,  0.799537269f, 0.605511041f,  0.795836905f, 0.610382806f,  0.792106577f, 0.615231591f,  0.788346428f, 0.620057212f,  0.784556597f, 0.624859488f,  0.780737229f, 0.629638239f,  0.776888466f, 0.634393284f,  0.773010453f, 0.639124445f,  0.769103338f, 0.643831543f,  0.765167266f, 0.648514401f,  0.761202385f, 0.653172843f,  0.757208847f, 0.657806693f,  0.753186799f, 0.662415778f,  0.749136395f, 0.666999922f,  0.745057785f, 0.671558955f,  0.740951125f, 0.676092704f,  0.736816569f, 0.680600998f,  0.732654272f, 0.685083668f,  0.728464390f, 0.689540545f,  0.724247083f, 0.693971461f,  0.720002508f, 0.698376249f,  0.715730825f, 0.702754744f,  0.711432196f, 0.707106781f,  0.707106781f, 0.711432196f,  0.702754744f, 0.715730825f,  0.698376249f, 0.720002508f,  0.693971461f, 0.724247083f,  0.689540545f, 0.728464390f,  0.685083668f, 0.732654272f,  0.680600998f, 0.736816569f,  0.676092704f, 0.740951125f,  0.671558955f, 0.745057785f,  0.666999922f, 0.749136395f,  0.662415778f, 0.753186799f,  0.657806693f, 0.757208847f,  0.653172843f, 0.761202385f,  0.648514401f, 0.765167266f,  0.643831543f, 0.769103338f,  0.639124445f, 0.773010453f,  0.634393284f, 0.776888466f,  0.629638239f, 0.780737229f,  0.624859488f, 0.784556597f,  0.620057212f, 0.788346428f,  0.615231591f, 0.792106577f,  0.610382806f, 0.795836905f,  0.605511041f, 0.799537269f,  0.600616479f, 0.803207531f,  0.595699304f, 0.806847554f,  0.590759702f, 0.810457198f,  0.585797857f, 0.814036330f,  0.580813958f, 0.817584813f,  0.575808191f, 0.821102515f,  0.570780746f, 0.824589303f,  0.565731811f, 0.828045045f,  0.560661576f, 0.831469612f,  0.555570233f, 0.834862875f,  0.550457973f, 0.838224706f,  0.545324988f, 0.841554977f,  0.540171473f, 0.844853565f,  0.534997620f, 0.848120345f,  0.529803625f, 0.851355193f,  0.524589683f, 0.854557988f,  0.519355990f, 0.857728610f,  0.514102744f, 0.860866939f,  0.508830143f, 0.863972856f,  0.503538384f, 0.867046246f,  0.498227667f, 0.870086991f,  0.492898192f, 0.873094978f,  0.487550160f, 0.876070094f,  0.482183772f, 0.879012226f,  0.476799230f, 0.881921264f,  0.471396737f, 0.884797098f,  0.465976496f, 0.887639620f,  0.460538711f, 0.890448723f,  0.455083587f, 0.893224301f,  0.449611330f, 0.895966250f,  0.444122145f, 0.898674466f,  0.438616239f, 0.901348847f,  0.433093819f, 0.903989293f,  0.427555093f, 0.906595705f,  0.422000271f, 0.909167983f,  0.416429560f, 0.911706032f,  0.410843171f, 0.914209756f,  0.405241314f, 0.916679060f,  0.399624200f, 0.919113852f,  0.393992040f, 0.921514039f,  0.388345047f, 0.923879533f,  0.382683432f, 0.926210242f,  0.377007410f, 0.928506080f,  0.371317194f, 0.930766961f,  0.365612998f, 0.932992799f,  0.359895037f, 0.935183510f,  0.354163525f, 0.937339012f,  0.348418680f, 0.939459224f,  0.342660717f, 0.941544065f,  0.336889853f, 0.943593458f,  0.331106306f, 0.945607325f,  0.325310292f, 0.947585591f,  0.319502031f, 0.949528181f,  0.313681740f, 0.951435021f,  0.307849640f, 0.953306040f,  0.302005949f, 0.955141168f,  0.296150888f, 0.956940336f,  0.290284677f, 0.958703475f,  0.284407537f, 0.960430519f,  0.278519689f, 0.962121404f,  0.272621355f, 0.963776066f,  0.266712757f, 0.965394442f,  0.260794118f, 0.966976471f,  0.254865660f, 0.968522094f,  0.248927606f, 0.970031253f,  0.242980180f, 0.971503891f,  0.237023606f, 0.972939952f,  0.231058108f, 0.974339383f,  0.225083911f, 0.975702130f,  0.219101240f, 0.977028143f,  0.213110320f, 0.978317371f,  0.207111376f, 0.979569766f,  0.201104635f, 0.980785280f,  0.195090322f, 0.981963869f,  0.189068664f, 0.983105487f,  0.183039888f, 0.984210092f,  0.177004220f, 0.985277642f,  0.170961889f, 0.986308097f,  0.164913120f, 0.987301418f,  0.158858143f, 0.988257568f,  0.152797185f, 0.989176510f,  0.146730474f, 0.990058210f,  0.140658239f, 0.990902635f,  0.134580709f, 0.991709754f,  0.128498111f, 0.992479535f,  0.122410675f, 0.993211949f,  0.116318631f, 0.993906970f,  0.110222207f, 0.994564571f,  0.104121634f, 0.995184727f,  0.098017140f, 0.995767414f,  0.091908956f, 0.996312612f,  0.085797312f, 0.996820299f,  0.079682438f, 0.997290457f,  0.073564564f, 0.997723067f,  0.067443920f, 0.998118113f,  0.061320736f, 0.998475581f,  0.055195244f, 0.998795456f,  0.049067674f, 0.999077728f,  0.042938257f, 0.999322385f,  0.036807223f, 0.999529418f,  0.030674803f, 0.999698819f,  0.024541229f, 0.999830582f,  0.018406730f, 0.999924702f,  0.012271538f, 0.999981175f,  0.006135885f, 1.000000000f,  0.000000000f, 0.999981175f, -0.006135885f, 0.999924702f, -0.012271538f, 0.999830582f, -0.018406730f, 0.999698819f, -0.024541229f, 0.999529418f, -0.030674803f, 0.999322385f, -0.036807223f, 0.999077728f, -0.042938257f, 0.998795456f, -0.049067674f, 0.998475581f, -0.055195244f, 0.998118113f, -0.061320736f, 0.997723067f, -0.067443920f, 0.997290457f, -0.073564564f, 0.996820299f, -0.079682438f, 0.996312612f, -0.085797312f, 0.995767414f, -0.091908956f, 0.995184727f, -0.098017140f, 0.994564571f, -0.104121634f, 0.993906970f, -0.110222207f, 0.993211949f, -0.116318631f, 0.992479535f, -0.122410675f, 0.991709754f, -0.128498111f, 0.990902635f, -0.134580709f, 0.990058210f, -0.140658239f, 0.989176510f, -0.146730474f, 0.988257568f, -0.152797185f, 0.987301418f, -0.158858143f, 0.986308097f, -0.164913120f, 0.985277642f, -0.170961889f, 0.984210092f, -0.177004220f, 0.983105487f, -0.183039888f, 0.981963869f, -0.189068664f, 0.980785280f, -0.195090322f, 0.979569766f, -0.201104635f, 0.978317371f, -0.207111376f, 0.977028143f, -0.213110320f, 0.975702130f, -0.219101240f, 0.974339383f, -0.225083911f, 0.972939952f, -0.231058108f, 0.971503891f, -0.237023606f, 0.970031253f, -0.242980180f, 0.968522094f, -0.248927606f, 0.966976471f, -0.254865660f, 0.965394442f, -0.260794118f, 0.963776066f, -0.266712757f, 0.962121404f, -0.272621355f, 0.960430519f, -0.278519689f, 0.958703475f, -0.284407537f, 0.956940336f, -0.290284677f, 0.955141168f, -0.296150888f, 0.953306040f, -0.302005949f, 0.951435021f, -0.307849640f, 0.949528181f, -0.313681740f, 0.947585591f, -0.319502031f, 0.945607325f, -0.325310292f, 0.943593458f, -0.331106306f, 0.941544065f, -0.336889853f, 0.939459224f, -0.342660717f, 0.937339012f, -0.348418680f, 0.935183510f, -0.354163525f, 0.932992799f, -0.359895037f, 0.930766961f, -0.365612998f, 0.928506080f, -0.371317194f, 0.926210242f, -0.377007410f, 0.923879533f, -0.382683432f, 0.921514039f, -0.388345047f, 0.919113852f, -0.393992040f, 0.916679060f, -0.399624200f, 0.914209756f, -0.405241314f, 0.911706032f, -0.410843171f, 0.909167983f, -0.416429560f, 0.906595705f, -0.422000271f, 0.903989293f, -0.427555093f, 0.901348847f, -0.433093819f, 0.898674466f, -0.438616239f, 0.895966250f, -0.444122145f, 0.893224301f, -0.449611330f, 0.890448723f, -0.455083587f, 0.887639620f, -0.460538711f, 0.884797098f, -0.465976496f, 0.881921264f, -0.471396737f, 0.879012226f, -0.476799230f, 0.876070094f, -0.482183772f, 0.873094978f, -0.487550160f, 0.870086991f, -0.492898192f, 0.867046246f, -0.498227667f, 0.863972856f, -0.503538384f, 0.860866939f, -0.508830143f, 0.857728610f, -0.514102744f, 0.854557988f, -0.519355990f, 0.851355193f, -0.524589683f, 0.848120345f, -0.529803625f, 0.844853565f, -0.534997620f, 0.841554977f, -0.540171473f, 0.838224706f, -0.545324988f, 0.834862875f, -0.550457973f, 0.831469612f, -0.555570233f, 0.828045045f, -0.560661576f, 0.824589303f, -0.565731811f, 0.821102515f, -0.570780746f, 0.817584813f, -0.575808191f, 0.814036330f, -0.580813958f, 0.810457198f, -0.585797857f, 0.806847554f, -0.590759702f, 0.803207531f, -0.595699304f, 0.799537269f, -0.600616479f, 0.795836905f, -0.605511041f, 0.792106577f, -0.610382806f, 0.788346428f, -0.615231591f, 0.784556597f, -0.620057212f, 0.780737229f, -0.624859488f, 0.776888466f, -0.629638239f, 0.773010453f, -0.634393284f, 0.769103338f, -0.639124445f, 0.765167266f, -0.643831543f, 0.761202385f, -0.648514401f, 0.757208847f, -0.653172843f, 0.753186799f, -0.657806693f, 0.749136395f, -0.662415778f, 0.745057785f, -0.666999922f, 0.740951125f, -0.671558955f, 0.736816569f, -0.676092704f, 0.732654272f, -0.680600998f, 0.728464390f, -0.685083668f, 0.724247083f, -0.689540545f, 0.720002508f, -0.693971461f, 0.715730825f, -0.698376249f, 0.711432196f, -0.702754744f, 0.707106781f, -0.707106781f, 0.702754744f, -0.711432196f, 0.698376249f, -0.715730825f, 0.693971461f, -0.720002508f, 0.689540545f, -0.724247083f, 0.685083668f, -0.728464390f, 0.680600998f, -0.732654272f, 0.676092704f, -0.736816569f, 0.671558955f, -0.740951125f, 0.666999922f, -0.745057785f, 0.662415778f, -0.749136395f, 0.657806693f, -0.753186799f, 0.653172843f, -0.757208847f, 0.648514401f, -0.761202385f, 0.643831543f, -0.765167266f, 0.639124445f, -0.769103338f, 0.634393284f, -0.773010453f, 0.629638239f, -0.776888466f, 0.624859488f, -0.780737229f, 0.620057212f, -0.784556597f, 0.615231591f, -0.788346428f, 0.610382806f, -0.792106577f, 0.605511041f, -0.795836905f, 0.600616479f, -0.799537269f, 0.595699304f, -0.803207531f, 0.590759702f, -0.806847554f, 0.585797857f, -0.810457198f, 0.580813958f, -0.814036330f, 0.575808191f, -0.817584813f, 0.570780746f, -0.821102515f, 0.565731811f, -0.824589303f, 0.560661576f, -0.828045045f, 0.555570233f, -0.831469612f, 0.550457973f, -0.834862875f, 0.545324988f, -0.838224706f, 0.540171473f, -0.841554977f, 0.534997620f, -0.844853565f, 0.529803625f, -0.848120345f, 0.524589683f, -0.851355193f, 0.519355990f, -0.854557988f, 0.514102744f, -0.857728610f, 0.508830143f, -0.860866939f, 0.503538384f, -0.863972856f, 0.498227667f, -0.867046246f, 0.492898192f, -0.870086991f, 0.487550160f, -0.873094978f, 0.482183772f, -0.876070094f, 0.476799230f, -0.879012226f, 0.471396737f, -0.881921264f, 0.465976496f, -0.884797098f, 0.460538711f, -0.887639620f, 0.455083587f, -0.890448723f, 0.449611330f, -0.893224301f, 0.444122145f, -0.895966250f, 0.438616239f, -0.898674466f, 0.433093819f, -0.901348847f, 0.427555093f, -0.903989293f, 0.422000271f, -0.906595705f, 0.416429560f, -0.909167983f, 0.410843171f, -0.911706032f, 0.405241314f, -0.914209756f, 0.399624200f, -0.916679060f, 0.393992040f, -0.919113852f, 0.388345047f, -0.921514039f, 0.382683432f, -0.923879533f, 0.377007410f, -0.926210242f, 0.371317194f, -0.928506080f, 0.365612998f, -0.930766961f, 0.359895037f, -0.932992799f, 0.354163525f, -0.935183510f, 0.348418680f, -0.937339012f, 0.342660717f, -0.939459224f, 0.336889853f, -0.941544065f, 0.331106306f, -0.943593458f, 0.325310292f, -0.945607325f, 0.319502031f, -0.947585591f, 0.313681740f, -0.949528181f, 0.307849640f, -0.951435021f, 0.302005949f, -0.953306040f, 0.296150888f, -0.955141168f, 0.290284677f, -0.956940336f, 0.284407537f, -0.958703475f, 0.278519689f, -0.960430519f, 0.272621355f, -0.962121404f, 0.266712757f, -0.963776066f, 0.260794118f, -0.965394442f, 0.254865660f, -0.966976471f, 0.248927606f, -0.968522094f, 0.242980180f, -0.970031253f, 0.237023606f, -0.971503891f, 0.231058108f, -0.972939952f, 0.225083911f, -0.974339383f, 0.219101240f, -0.975702130f, 0.213110320f, -0.977028143f, 0.207111376f, -0.978317371f, 0.201104635f, -0.979569766f, 0.195090322f, -0.980785280f, 0.189068664f, -0.981963869f, 0.183039888f, -0.983105487f, 0.177004220f, -0.984210092f, 0.170961889f, -0.985277642f, 0.164913120f, -0.986308097f, 0.158858143f, -0.987301418f, 0.152797185f, -0.988257568f, 0.146730474f, -0.989176510f, 0.140658239f, -0.990058210f, 0.134580709f, -0.990902635f, 0.128498111f, -0.991709754f, 0.122410675f, -0.992479535f, 0.116318631f, -0.993211949f, 0.110222207f, -0.993906970f, 0.104121634f, -0.994564571f, 0.098017140f, -0.995184727f, 0.091908956f, -0.995767414f, 0.085797312f, -0.996312612f, 0.079682438f, -0.996820299f, 0.073564564f, -0.997290457f, 0.067443920f, -0.997723067f, 0.061320736f, -0.998118113f, 0.055195244f, -0.998475581f, 0.049067674f, -0.998795456f, 0.042938257f, -0.999077728f, 0.036807223f, -0.999322385f, 0.030674803f, -0.999529418f, 0.024541229f, -0.999698819f, 0.018406730f, -0.999830582f, 0.012271538f, -0.999924702f, 0.006135885f, -0.999981175f }

#define AUDIO_LEN 16000
#define FRAME_LEN 640
#define FRAME_LEN_PADDED 1024
#define FRAME_SHIFT 320

#define NUM_BINS 513
#define NUM_FRAMES 49
#define RECORDING_WIN 49
#define SLIDING_WINDOW_LEN 1


#ifdef __cplusplus
extern "C" {
#endif
void mfcc_init();
void mfcc_compute(const int16_t * audio_data, float * freq_data);
#ifdef __cplusplus
}
#endif

// #ifdef __cplusplus
// extern "C" {
// #endif
// #ifdef __cplusplus
// }
// #endif



#endif
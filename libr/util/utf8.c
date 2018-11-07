/* radare2 - LGPL - Copyright 2014-2018 - thelemon, kazarmy, pancake */

#include <r_types.h>
#include <r_util.h>

static const struct { ut32 from, to; } nonprintable_ranges[] = {
	{ 0x0000, 0x001F }, { 0x007F, 0x009F }, { 0x034F, 0x034F },
	{ 0x0378, 0x0379 }, { 0x037F, 0x0383 }, { 0x038B, 0x038B },
	{ 0x038D, 0x038D }, { 0x03A2, 0x03A2 }, { 0x0528, 0x0530 },
	{ 0x0557, 0x0558 }, { 0x0560, 0x0560 }, { 0x0588, 0x0588 },
	{ 0x058B, 0x058E }, { 0x0590, 0x0590 }, { 0x05C8, 0x05CF },
	{ 0x05EB, 0x05EF }, { 0x05F5, 0x0605 }, { 0x061C, 0x061D },
	{ 0x06DD, 0x06DD }, { 0x070E, 0x070F }, { 0x074B, 0x074C },
	{ 0x07B2, 0x07BF }, { 0x07FB, 0x07FF }, { 0x082E, 0x082F },
	{ 0x083F, 0x083F }, { 0x085C, 0x085D }, { 0x085F, 0x089F },
	{ 0x08A1, 0x08A1 }, { 0x08AD, 0x08E3 }, { 0x08FF, 0x08FF },
	{ 0x0978, 0x0978 }, { 0x0980, 0x0980 }, { 0x0984, 0x0984 },
	{ 0x098D, 0x098E }, { 0x0991, 0x0992 }, { 0x09A9, 0x09A9 },
	{ 0x09B1, 0x09B1 }, { 0x09B3, 0x09B5 }, { 0x09BA, 0x09BB },
	{ 0x09C5, 0x09C6 }, { 0x09C9, 0x09CA }, { 0x09CF, 0x09D6 },
	{ 0x09D8, 0x09DB }, { 0x09DE, 0x09DE }, { 0x09E4, 0x09E5 },
	{ 0x09FC, 0x0A00 }, { 0x0A04, 0x0A04 }, { 0x0A0B, 0x0A0E },
	{ 0x0A11, 0x0A12 }, { 0x0A29, 0x0A29 }, { 0x0A31, 0x0A31 },
	{ 0x0A34, 0x0A34 }, { 0x0A37, 0x0A37 }, { 0x0A3A, 0x0A3B },
	{ 0x0A3D, 0x0A3D }, { 0x0A43, 0x0A46 }, { 0x0A49, 0x0A4A },
	{ 0x0A4E, 0x0A50 }, { 0x0A52, 0x0A58 }, { 0x0A5D, 0x0A5D },
	{ 0x0A5F, 0x0A65 }, { 0x0A76, 0x0A80 }, { 0x0A84, 0x0A84 },
	{ 0x0A8E, 0x0A8E }, { 0x0A92, 0x0A92 }, { 0x0AA9, 0x0AA9 },
	{ 0x0AB1, 0x0AB1 }, { 0x0AB4, 0x0AB4 }, { 0x0ABA, 0x0ABB },
	{ 0x0AC6, 0x0AC6 }, { 0x0ACA, 0x0ACA }, { 0x0ACE, 0x0ACF },
	{ 0x0AD1, 0x0ADF }, { 0x0AE4, 0x0AE5 }, { 0x0AF2, 0x0B00 },
	{ 0x0B04, 0x0B04 }, { 0x0B0D, 0x0B0E }, { 0x0B11, 0x0B12 },
	{ 0x0B29, 0x0B29 }, { 0x0B31, 0x0B31 }, { 0x0B34, 0x0B34 },
	{ 0x0B3A, 0x0B3B }, { 0x0B45, 0x0B46 }, { 0x0B49, 0x0B4A },
	{ 0x0B4E, 0x0B55 }, { 0x0B58, 0x0B5B }, { 0x0B5E, 0x0B5E },
	{ 0x0B64, 0x0B65 }, { 0x0B78, 0x0B81 }, { 0x0B84, 0x0B84 },
	{ 0x0B8B, 0x0B8D }, { 0x0B91, 0x0B91 }, { 0x0B96, 0x0B98 },
	{ 0x0B9B, 0x0B9B }, { 0x0B9D, 0x0B9D }, { 0x0BA0, 0x0BA2 },
	{ 0x0BA5, 0x0BA7 }, { 0x0BAB, 0x0BAD }, { 0x0BBA, 0x0BBD },
	{ 0x0BC3, 0x0BC5 }, { 0x0BC9, 0x0BC9 }, { 0x0BCE, 0x0BCF },
	{ 0x0BD1, 0x0BD6 }, { 0x0BD8, 0x0BE5 }, { 0x0BFB, 0x0C00 },
	{ 0x0C04, 0x0C04 }, { 0x0C0D, 0x0C0D }, { 0x0C11, 0x0C11 },
	{ 0x0C29, 0x0C29 }, { 0x0C34, 0x0C34 }, { 0x0C3A, 0x0C3C },
	{ 0x0C45, 0x0C45 }, { 0x0C49, 0x0C49 }, { 0x0C4E, 0x0C54 },
	{ 0x0C57, 0x0C57 }, { 0x0C5A, 0x0C5F }, { 0x0C64, 0x0C65 },
	{ 0x0C70, 0x0C77 }, { 0x0C80, 0x0C81 }, { 0x0C84, 0x0C84 },
	{ 0x0C8D, 0x0C8D }, { 0x0C91, 0x0C91 }, { 0x0CA9, 0x0CA9 },
	{ 0x0CB4, 0x0CB4 }, { 0x0CBA, 0x0CBB }, { 0x0CC5, 0x0CC5 },
	{ 0x0CC9, 0x0CC9 }, { 0x0CCE, 0x0CD4 }, { 0x0CD7, 0x0CDD },
	{ 0x0CDF, 0x0CDF }, { 0x0CE4, 0x0CE5 }, { 0x0CF0, 0x0CF0 },
	{ 0x0CF3, 0x0D01 }, { 0x0D04, 0x0D04 }, { 0x0D0D, 0x0D0D },
	{ 0x0D11, 0x0D11 }, { 0x0D3B, 0x0D3C }, { 0x0D45, 0x0D45 },
	{ 0x0D49, 0x0D49 }, { 0x0D4F, 0x0D56 }, { 0x0D58, 0x0D5F },
	{ 0x0D64, 0x0D65 }, { 0x0D76, 0x0D78 }, { 0x0D80, 0x0D81 },
	{ 0x0D84, 0x0D84 }, { 0x0D97, 0x0D99 }, { 0x0DB2, 0x0DB2 },
	{ 0x0DBC, 0x0DBC }, { 0x0DBE, 0x0DBF }, { 0x0DC7, 0x0DC9 },
	{ 0x0DCB, 0x0DCE }, { 0x0DD5, 0x0DD5 }, { 0x0DD7, 0x0DD7 },
	{ 0x0DE0, 0x0DF1 }, { 0x0DF5, 0x0E00 }, { 0x0E3B, 0x0E3E },
	{ 0x0E5C, 0x0E80 }, { 0x0E83, 0x0E83 }, { 0x0E85, 0x0E86 },
	{ 0x0E89, 0x0E89 }, { 0x0E8B, 0x0E8C }, { 0x0E8E, 0x0E93 },
	{ 0x0E98, 0x0E98 }, { 0x0EA0, 0x0EA0 }, { 0x0EA4, 0x0EA4 },
	{ 0x0EA6, 0x0EA6 }, { 0x0EA8, 0x0EA9 }, { 0x0EAC, 0x0EAC },
	{ 0x0EBA, 0x0EBA }, { 0x0EBE, 0x0EBF }, { 0x0EC5, 0x0EC5 },
	{ 0x0EC7, 0x0EC7 }, { 0x0ECE, 0x0ECF }, { 0x0EDA, 0x0EDB },
	{ 0x0EE0, 0x0EFF }, { 0x0F48, 0x0F48 }, { 0x0F6D, 0x0F70 },
	{ 0x0F98, 0x0F98 }, { 0x0FBD, 0x0FBD }, { 0x0FCD, 0x0FCD },
	{ 0x0FDB, 0x0FFF }, { 0x10C6, 0x10C6 }, { 0x10C8, 0x10CC },
	{ 0x10CE, 0x10CF }, { 0x115F, 0x1160 }, { 0x1249, 0x1249 },
	{ 0x124E, 0x124F }, { 0x1257, 0x1257 }, { 0x1259, 0x1259 },
	{ 0x125E, 0x125F }, { 0x1289, 0x1289 }, { 0x128E, 0x128F },
	{ 0x12B1, 0x12B1 }, { 0x12B6, 0x12B7 }, { 0x12BF, 0x12BF },
	{ 0x12C1, 0x12C1 }, { 0x12C6, 0x12C7 }, { 0x12D7, 0x12D7 },
	{ 0x1311, 0x1311 }, { 0x1316, 0x1317 }, { 0x135B, 0x135C },
	{ 0x137D, 0x137F }, { 0x139A, 0x139F }, { 0x13F5, 0x13FF },
	{ 0x169D, 0x169F }, { 0x16F1, 0x16FF }, { 0x170D, 0x170D },
	{ 0x1715, 0x171F }, { 0x1737, 0x173F }, { 0x1754, 0x175F },
	{ 0x176D, 0x176D }, { 0x1771, 0x1771 }, { 0x1774, 0x177F },
	{ 0x17B4, 0x17B5 }, { 0x17DE, 0x17DF }, { 0x17EA, 0x17EF },
	{ 0x17FA, 0x17FF }, { 0x180B, 0x180D }, { 0x180F, 0x180F },
	{ 0x181A, 0x181F }, { 0x1878, 0x187F }, { 0x18AB, 0x18AF },
	{ 0x18F6, 0x18FF }, { 0x191D, 0x191F }, { 0x192C, 0x192F },
	{ 0x193C, 0x193F }, { 0x1941, 0x1943 }, { 0x196E, 0x196F },
	{ 0x1975, 0x197F }, { 0x19AC, 0x19AF }, { 0x19CA, 0x19CF },
	{ 0x19DB, 0x19DD }, { 0x1A1C, 0x1A1D }, { 0x1A5F, 0x1A5F },
	{ 0x1A7D, 0x1A7E }, { 0x1A8A, 0x1A8F }, { 0x1A9A, 0x1A9F },
	{ 0x1AAE, 0x1AFF }, { 0x1B4C, 0x1B4F }, { 0x1B7D, 0x1B7F },
	{ 0x1BF4, 0x1BFB }, { 0x1C38, 0x1C3A }, { 0x1C4A, 0x1C4C },
	{ 0x1C80, 0x1CBF }, { 0x1CC8, 0x1CCF }, { 0x1CF7, 0x1CFF },
	{ 0x1DE7, 0x1DFB }, { 0x1F16, 0x1F17 }, { 0x1F1E, 0x1F1F },
	{ 0x1F46, 0x1F47 }, { 0x1F4E, 0x1F4F }, { 0x1F58, 0x1F58 },
	{ 0x1F5A, 0x1F5A }, { 0x1F5C, 0x1F5C }, { 0x1F5E, 0x1F5E },
	{ 0x1F7E, 0x1F7F }, { 0x1FB5, 0x1FB5 }, { 0x1FC5, 0x1FC5 },
	{ 0x1FD4, 0x1FD5 }, { 0x1FDC, 0x1FDC }, { 0x1FF0, 0x1FF1 },
	{ 0x1FF5, 0x1FF5 }, { 0x1FFF, 0x1FFF }, { 0x200B, 0x200F },
	{ 0x202A, 0x202E }, { 0x2060, 0x206F }, { 0x2072, 0x2073 },
	{ 0x208F, 0x208F }, { 0x209D, 0x209F }, { 0x20BB, 0x20CF },
	{ 0x20F1, 0x20FF }, { 0x218A, 0x218F }, { 0x23F4, 0x23FF },
	{ 0x2427, 0x243F }, { 0x244B, 0x245F }, { 0x2700, 0x2700 },
	{ 0x2B4D, 0x2B4F }, { 0x2B5A, 0x2BFF }, { 0x2C2F, 0x2C2F },
	{ 0x2C5F, 0x2C5F }, { 0x2CF4, 0x2CF8 }, { 0x2D26, 0x2D26 },
	{ 0x2D28, 0x2D2C }, { 0x2D2E, 0x2D2F }, { 0x2D68, 0x2D6E },
	{ 0x2D71, 0x2D7E }, { 0x2D97, 0x2D9F }, { 0x2DA7, 0x2DA7 },
	{ 0x2DAF, 0x2DAF }, { 0x2DB7, 0x2DB7 }, { 0x2DBF, 0x2DBF },
	{ 0x2DC7, 0x2DC7 }, { 0x2DCF, 0x2DCF }, { 0x2DD7, 0x2DD7 },
	{ 0x2DDF, 0x2DDF }, { 0x2E3C, 0x2E7F }, { 0x2E9A, 0x2E9A },
	{ 0x2EF4, 0x2EFF }, { 0x2FD6, 0x2FEF }, { 0x2FFC, 0x2FFF },
	{ 0x3040, 0x3040 }, { 0x3097, 0x3098 }, { 0x3100, 0x3104 },
	{ 0x312E, 0x3130 }, { 0x3164, 0x3164 }, { 0x318F, 0x318F },
	{ 0x31BB, 0x31BF }, { 0x31E4, 0x31EF }, { 0x321F, 0x321F },
	{ 0x32FF, 0x32FF }, { 0x4DB6, 0x4DBF }, { 0x9FCD, 0x9FFF },
	{ 0xA48D, 0xA48F }, { 0xA4C7, 0xA4CF }, { 0xA62C, 0xA63F },
	{ 0xA698, 0xA69E }, { 0xA6F8, 0xA6FF }, { 0xA78F, 0xA78F },
	{ 0xA794, 0xA79F }, { 0xA7AB, 0xA7F7 }, { 0xA82C, 0xA82F },
	{ 0xA83A, 0xA83F }, { 0xA878, 0xA87F }, { 0xA8C5, 0xA8CD },
	{ 0xA8DA, 0xA8DF }, { 0xA8FC, 0xA8FF }, { 0xA954, 0xA95E },
	{ 0xA97D, 0xA97F }, { 0xA9CE, 0xA9CE }, { 0xA9DA, 0xA9DD },
	{ 0xA9E0, 0xA9FF }, { 0xAA37, 0xAA3F }, { 0xAA4E, 0xAA4F },
	{ 0xAA5A, 0xAA5B }, { 0xAA7C, 0xAA7F }, { 0xAAC3, 0xAADA },
	{ 0xAAF7, 0xAB00 }, { 0xAB07, 0xAB08 }, { 0xAB0F, 0xAB10 },
	{ 0xAB17, 0xAB1F }, { 0xAB27, 0xAB27 }, { 0xAB2F, 0xABBF },
	{ 0xABEE, 0xABEF }, { 0xABFA, 0xABFF }, { 0xD7A4, 0xD7AF },
	{ 0xD7C7, 0xD7CA }, { 0xD7FC, 0xDFFF }, { 0xFA6E, 0xFA6F },
	{ 0xFADA, 0xFAFF }, { 0xFB07, 0xFB12 }, { 0xFB18, 0xFB1C },
	{ 0xFB37, 0xFB37 }, { 0xFB3D, 0xFB3D }, { 0xFB3F, 0xFB3F },
	{ 0xFB42, 0xFB42 }, { 0xFB45, 0xFB45 }, { 0xFBC2, 0xFBD2 },
	{ 0xFD40, 0xFD4F }, { 0xFD90, 0xFD91 }, { 0xFDC8, 0xFDEF },
	{ 0xFDFE, 0xFE0F }, { 0xFE1A, 0xFE1F }, { 0xFE27, 0xFE2F },
	{ 0xFE53, 0xFE53 }, { 0xFE67, 0xFE67 }, { 0xFE6C, 0xFE6F },
	{ 0xFE75, 0xFE75 }, { 0xFEFD, 0xFEFF }, { 0xFF00, 0xFF00 },
	{ 0xFFA0, 0xFFA0 }, { 0xFFBF, 0xFFC1 }, { 0xFFC8, 0xFFC9 },
	{ 0xFFD0, 0xFFD1 }, { 0xFFD8, 0xFFD9 }, { 0xFFDD, 0xFFDF },
	{ 0xFFE7, 0xFFE7 }, { 0xFFEF, 0xFFFB }, { 0xFFFE, 0xFFFF },
	{ 0x1000C, 0x1000C }, { 0x10027, 0x10027 }, { 0x1003B, 0x1003B },
	{ 0x1003E, 0x1003E }, { 0x1004E, 0x1004F }, { 0x1005E, 0x1007F },
	{ 0x100FB, 0x100FF }, { 0x10103, 0x10106 }, { 0x10134, 0x10136 },
	{ 0x1018B, 0x1018F }, { 0x1019C, 0x101CF }, { 0x101FE, 0x1027F },
	{ 0x1029D, 0x1029F }, { 0x102D1, 0x102FF }, { 0x1031F, 0x1031F },
	{ 0x10324, 0x1032F }, { 0x1034B, 0x1037F }, { 0x1039E, 0x1039E },
	{ 0x103C4, 0x103C7 }, { 0x103D6, 0x103FF }, { 0x1049E, 0x1049F },
	{ 0x104AA, 0x107FF }, { 0x10806, 0x10807 }, { 0x10809, 0x10809 },
	{ 0x10836, 0x10836 }, { 0x10839, 0x1083B }, { 0x1083D, 0x1083E },
	{ 0x10856, 0x10856 }, { 0x10860, 0x108FF }, { 0x1091C, 0x1091E },
	{ 0x1093A, 0x1093E }, { 0x10940, 0x1097F }, { 0x109B8, 0x109BD },
	{ 0x109C0, 0x109FF }, { 0x10A04, 0x10A04 }, { 0x10A07, 0x10A0B },
	{ 0x10A14, 0x10A14 }, { 0x10A18, 0x10A18 }, { 0x10A34, 0x10A37 },
	{ 0x10A3B, 0x10A3E }, { 0x10A48, 0x10A4F }, { 0x10A59, 0x10A5F },
	{ 0x10A80, 0x10AFF }, { 0x10B36, 0x10B38 }, { 0x10B56, 0x10B57 },
	{ 0x10B73, 0x10B77 }, { 0x10B80, 0x10BFF }, { 0x10C49, 0x10E5F },
	{ 0x10E7F, 0x10FFF }, { 0x1104E, 0x11051 }, { 0x11070, 0x1107F },
	{ 0x110BD, 0x110BD }, { 0x110C2, 0x110CF }, { 0x110E9, 0x110EF },
	{ 0x110FA, 0x110FF }, { 0x11135, 0x11135 }, { 0x11144, 0x1117F },
	{ 0x111C9, 0x111CF }, { 0x111DA, 0x1167F }, { 0x116B8, 0x116BF },
	{ 0x116CA, 0x11FFF }, { 0x1236F, 0x123FF }, { 0x12463, 0x1246F },
	{ 0x12474, 0x12FFF }, { 0x1342F, 0x167FF }, { 0x16A39, 0x16EFF },
	{ 0x16F45, 0x16F4F }, { 0x16F7F, 0x16F8E }, { 0x16FA0, 0x1AFFF },
	{ 0x1B002, 0x1CFFF }, { 0x1D0F6, 0x1D0FF }, { 0x1D127, 0x1D128 },
	{ 0x1D173, 0x1D17A }, { 0x1D1DE, 0x1D1FF }, { 0x1D246, 0x1D2FF },
	{ 0x1D357, 0x1D35F }, { 0x1D372, 0x1D3FF }, { 0x1D455, 0x1D455 },
	{ 0x1D49D, 0x1D49D }, { 0x1D4A0, 0x1D4A1 }, { 0x1D4A3, 0x1D4A4 },
	{ 0x1D4A7, 0x1D4A8 }, { 0x1D4AD, 0x1D4AD }, { 0x1D4BA, 0x1D4BA },
	{ 0x1D4BC, 0x1D4BC }, { 0x1D4C4, 0x1D4C4 }, { 0x1D506, 0x1D506 },
	{ 0x1D50B, 0x1D50C }, { 0x1D515, 0x1D515 }, { 0x1D51D, 0x1D51D },
	{ 0x1D53A, 0x1D53A }, { 0x1D53F, 0x1D53F }, { 0x1D545, 0x1D545 },
	{ 0x1D547, 0x1D549 }, { 0x1D551, 0x1D551 }, { 0x1D6A6, 0x1D6A7 },
	{ 0x1D7CC, 0x1D7CD }, { 0x1D800, 0x1EDFF }, { 0x1EE04, 0x1EE04 },
	{ 0x1EE20, 0x1EE20 }, { 0x1EE23, 0x1EE23 }, { 0x1EE25, 0x1EE26 },
	{ 0x1EE28, 0x1EE28 }, { 0x1EE33, 0x1EE33 }, { 0x1EE38, 0x1EE38 },
	{ 0x1EE3A, 0x1EE3A }, { 0x1EE3C, 0x1EE41 }, { 0x1EE43, 0x1EE46 },
	{ 0x1EE48, 0x1EE48 }, { 0x1EE4A, 0x1EE4A }, { 0x1EE4C, 0x1EE4C },
	{ 0x1EE50, 0x1EE50 }, { 0x1EE53, 0x1EE53 }, { 0x1EE55, 0x1EE56 },
	{ 0x1EE58, 0x1EE58 }, { 0x1EE5A, 0x1EE5A }, { 0x1EE5C, 0x1EE5C },
	{ 0x1EE5E, 0x1EE5E }, { 0x1EE60, 0x1EE60 }, { 0x1EE63, 0x1EE63 },
	{ 0x1EE65, 0x1EE66 }, { 0x1EE6B, 0x1EE6B }, { 0x1EE73, 0x1EE73 },
	{ 0x1EE78, 0x1EE78 }, { 0x1EE7D, 0x1EE7D }, { 0x1EE7F, 0x1EE7F },
	{ 0x1EE8A, 0x1EE8A }, { 0x1EE9C, 0x1EEA0 }, { 0x1EEA4, 0x1EEA4 },
	{ 0x1EEAA, 0x1EEAA }, { 0x1EEBC, 0x1EEEF }, { 0x1EEF2, 0x1EFFF },
	{ 0x1F02C, 0x1F02F }, { 0x1F094, 0x1F09F }, { 0x1F0AF, 0x1F0B0 },
	{ 0x1F0BF, 0x1F0C0 }, { 0x1F0D0, 0x1F0D0 }, { 0x1F0E0, 0x1F0FF },
	{ 0x1F10B, 0x1F10F }, { 0x1F12F, 0x1F12F }, { 0x1F16C, 0x1F16F },
	{ 0x1F19B, 0x1F1E5 }, { 0x1F203, 0x1F20F }, { 0x1F23B, 0x1F23F },
	{ 0x1F249, 0x1F24F }, { 0x1F252, 0x1F2FF }, { 0x1F321, 0x1F32F },
	{ 0x1F336, 0x1F336 }, { 0x1F37D, 0x1F37F }, { 0x1F394, 0x1F39F },
	{ 0x1F3C5, 0x1F3C5 }, { 0x1F3CB, 0x1F3DF }, { 0x1F3F1, 0x1F3FF },
	{ 0x1F43F, 0x1F43F }, { 0x1F441, 0x1F441 }, { 0x1F4F8, 0x1F4F8 },
	{ 0x1F4FD, 0x1F4FF }, { 0x1F53E, 0x1F53F }, { 0x1F544, 0x1F54F },
	{ 0x1F568, 0x1F5FA }, { 0x1F641, 0x1F644 }, { 0x1F650, 0x1F67F },
	{ 0x1F6C6, 0x1F6FF }, { 0x1F774, 0x1FFFF }, { 0x2A6D7, 0x2A6FF },
	{ 0x2B735, 0x2B73F }, { 0x2B81E, 0x2F7FF }, { 0x2FA1E, 0xF0000 },
	{ 0xFFFFE, 0xFFFFF }, { 0x10FFFE, 0x10FFFF }, { 0x110000, 0xFFFFFFFF }
};
static const int nonprintable_ranges_count = sizeof (nonprintable_ranges) / sizeof (nonprintable_ranges[0]);

static const int lastUtfBlock = 281;

const RUtfBlock r_utf_blocks[] = {
	{ 0x0000, 0x007F, "Basic Latin" },
	{ 0x0080, 0x00FF, "Latin-1 Supplement" },
	{ 0x0100, 0x017F, "Latin Extended-A" },
	{ 0x0180, 0x024F, "Latin Extended-B" },
	{ 0x0250, 0x02AF, "IPA Extensions" },
	{ 0x02B0, 0x02FF, "Spacing Modifier Letters" },
	{ 0x0300, 0x036F, "Combining Diacritical Marks" },
	{ 0x0370, 0x03FF, "Greek and Coptic" },
	{ 0x0400, 0x04FF, "Cyrillic" },
	{ 0x0500, 0x052F, "Cyrillic Supplement" },
	{ 0x0530, 0x058F, "Armenian" },
	{ 0x0590, 0x05FF, "Hebrew" },
	{ 0x0600, 0x06FF, "Arabic" },
	{ 0x0700, 0x074F, "Syriac" },
	{ 0x0750, 0x077F, "Arabic Supplement" },
	{ 0x0780, 0x07BF, "Thaana" },
	{ 0x07C0, 0x07FF, "NKo" },
	{ 0x0800, 0x083F, "Samaritan" },
	{ 0x0840, 0x085F, "Mandaic" },
	{ 0x0860, 0x086F, "Syriac Supplement" },
	{ 0x08A0, 0x08FF, "Arabic Extended-A" },
	{ 0x0900, 0x097F, "Devanagari" },
	{ 0x0980, 0x09FF, "Bengali" },
	{ 0x0A00, 0x0A7F, "Gurmukhi" },
	{ 0x0A80, 0x0AFF, "Gujarati" },
	{ 0x0B00, 0x0B7F, "Oriya" },
	{ 0x0B80, 0x0BFF, "Tamil" },
	{ 0x0C00, 0x0C7F, "Telugu" },
	{ 0x0C80, 0x0CFF, "Kannada" },
	{ 0x0D00, 0x0D7F, "Malayalam" },
	{ 0x0D80, 0x0DFF, "Sinhala" },
	{ 0x0E00, 0x0E7F, "Thai" },
	{ 0x0E80, 0x0EFF, "Lao" },
	{ 0x0F00, 0x0FFF, "Tibetan" },
	{ 0x1000, 0x109F, "Myanmar" },
	{ 0x10A0, 0x10FF, "Georgian" },
	{ 0x1100, 0x11FF, "Hangul Jamo" },
	{ 0x1200, 0x137F, "Ethiopic" },
	{ 0x1380, 0x139F, "Ethiopic Supplement" },
	{ 0x13A0, 0x13FF, "Cherokee" },
	{ 0x1400, 0x167F, "Unified Canadian Aboriginal Syllabics" },
	{ 0x1680, 0x169F, "Ogham" },
	{ 0x16A0, 0x16FF, "Runic" },
	{ 0x1700, 0x171F, "Tagalog" },
	{ 0x1720, 0x173F, "Hanunoo" },
	{ 0x1740, 0x175F, "Buhid" },
	{ 0x1760, 0x177F, "Tagbanwa" },
	{ 0x1780, 0x17FF, "Khmer" },
	{ 0x1800, 0x18AF, "Mongolian" },
	{ 0x18B0, 0x18FF, "Unified Canadian Aboriginal Syllabics Extended" },
	{ 0x1900, 0x194F, "Limbu" },
	{ 0x1950, 0x197F, "Tai Le" },
	{ 0x1980, 0x19DF, "New Tai Lue" },
	{ 0x19E0, 0x19FF, "Khmer Symbols" },
	{ 0x1A00, 0x1A1F, "Buginese" },
	{ 0x1A20, 0x1AAF, "Tai Tham" },
	{ 0x1AB0, 0x1AFF, "Combining Diacritical Marks Extended" },
	{ 0x1B00, 0x1B7F, "Balinese" },
	{ 0x1B80, 0x1BBF, "Sundanese" },
	{ 0x1BC0, 0x1BFF, "Batak" },
	{ 0x1C00, 0x1C4F, "Lepcha" },
	{ 0x1C50, 0x1C7F, "Ol Chiki" },
	{ 0x1C80, 0x1C8F, "Cyrillic Extended-C" },
	{ 0x1CC0, 0x1CCF, "Sundanese Supplement" },
	{ 0x1CD0, 0x1CFF, "Vedic Extensions" },
	{ 0x1D00, 0x1D7F, "Phonetic Extensions" },
	{ 0x1D80, 0x1DBF, "Phonetic Extensions Supplement" },
	{ 0x1DC0, 0x1DFF, "Combining Diacritical Marks Supplement" },
	{ 0x1E00, 0x1EFF, "Latin Extended Additional" },
	{ 0x1F00, 0x1FFF, "Greek Extended" },
	{ 0x2000, 0x206F, "General Punctuation" },
	{ 0x2070, 0x209F, "Superscripts and Subscripts" },
	{ 0x20A0, 0x20CF, "Currency Symbols" },
	{ 0x20D0, 0x20FF, "Combining Diacritical Marks for Symbols" },
	{ 0x2100, 0x214F, "Letterlike Symbols" },
	{ 0x2150, 0x218F, "Number Forms" },
	{ 0x2190, 0x21FF, "Arrows" },
	{ 0x2200, 0x22FF, "Mathematical Operators" },
	{ 0x2300, 0x23FF, "Miscellaneous Technical" },
	{ 0x2400, 0x243F, "Control Pictures" },
	{ 0x2440, 0x245F, "Optical Character Recognition" },
	{ 0x2460, 0x24FF, "Enclosed Alphanumerics" },
	{ 0x2500, 0x257F, "Box Drawing" },
	{ 0x2580, 0x259F, "Block Elements" },
	{ 0x25A0, 0x25FF, "Geometric Shapes" },
	{ 0x2600, 0x26FF, "Miscellaneous Symbols" },
	{ 0x2700, 0x27BF, "Dingbats" },
	{ 0x27C0, 0x27EF, "Miscellaneous Mathematical Symbols-A" },
	{ 0x27F0, 0x27FF, "Supplemental Arrows-A" },
	{ 0x2800, 0x28FF, "Braille Patterns" },
	{ 0x2900, 0x297F, "Supplemental Arrows-B" },
	{ 0x2980, 0x29FF, "Miscellaneous Mathematical Symbols-B" },
	{ 0x2A00, 0x2AFF, "Supplemental Mathematical Operators" },
	{ 0x2B00, 0x2BFF, "Miscellaneous Symbols and Arrows" },
	{ 0x2C00, 0x2C5F, "Glagolitic" },
	{ 0x2C60, 0x2C7F, "Latin Extended-C" },
	{ 0x2C80, 0x2CFF, "Coptic" },
	{ 0x2D00, 0x2D2F, "Georgian Supplement" },
	{ 0x2D30, 0x2D7F, "Tifinagh" },
	{ 0x2D80, 0x2DDF, "Ethiopic Extended" },
	{ 0x2DE0, 0x2DFF, "Cyrillic Extended-A" },
	{ 0x2E00, 0x2E7F, "Supplemental Punctuation" },
	{ 0x2E80, 0x2EFF, "CJK Radicals Supplement" },
	{ 0x2F00, 0x2FDF, "Kangxi Radicals" },
	{ 0x2FF0, 0x2FFF, "Ideographic Description Characters" },
	{ 0x3000, 0x303F, "CJK Symbols and Punctuation" },
	{ 0x3040, 0x309F, "Hiragana" },
	{ 0x30A0, 0x30FF, "Katakana" },
	{ 0x3100, 0x312F, "Bopomofo" },
	{ 0x3130, 0x318F, "Hangul Compatibility Jamo" },
	{ 0x3190, 0x319F, "Kanbun" },
	{ 0x31A0, 0x31BF, "Bopomofo Extended" },
	{ 0x31C0, 0x31EF, "CJK Strokes" },
	{ 0x31F0, 0x31FF, "Katakana Phonetic Extensions" },
	{ 0x3200, 0x32FF, "Enclosed CJK Letters and Months" },
	{ 0x3300, 0x33FF, "CJK Compatibility" },
	{ 0x3400, 0x4DBF, "CJK Unified Ideographs Extension A" },
	{ 0x4DC0, 0x4DFF, "Yijing Hexagram Symbols" },
	{ 0x4E00, 0x9FFF, "CJK Unified Ideographs" },
	{ 0xA000, 0xA48F, "Yi Syllables" },
	{ 0xA490, 0xA4CF, "Yi Radicals" },
	{ 0xA4D0, 0xA4FF, "Lisu" },
	{ 0xA500, 0xA63F, "Vai" },
	{ 0xA640, 0xA69F, "Cyrillic Extended-B" },
	{ 0xA6A0, 0xA6FF, "Bamum" },
	{ 0xA700, 0xA71F, "Modifier Tone Letters" },
	{ 0xA720, 0xA7FF, "Latin Extended-D" },
	{ 0xA800, 0xA82F, "Syloti Nagri" },
	{ 0xA830, 0xA83F, "Common Indic Number Forms" },
	{ 0xA840, 0xA87F, "Phags-pa" },
	{ 0xA880, 0xA8DF, "Saurashtra" },
	{ 0xA8E0, 0xA8FF, "Devanagari Extended" },
	{ 0xA900, 0xA92F, "Kayah Li" },
	{ 0xA930, 0xA95F, "Rejang" },
	{ 0xA960, 0xA97F, "Hangul Jamo Extended-A" },
	{ 0xA980, 0xA9DF, "Javanese" },
	{ 0xA9E0, 0xA9FF, "Myanmar Extended-B" },
	{ 0xAA00, 0xAA5F, "Cham" },
	{ 0xAA60, 0xAA7F, "Myanmar Extended-A" },
	{ 0xAA80, 0xAADF, "Tai Viet" },
	{ 0xAAE0, 0xAAFF, "Meetei Mayek Extensions" },
	{ 0xAB00, 0xAB2F, "Ethiopic Extended-A" },
	{ 0xAB30, 0xAB6F, "Latin Extended-E" },
	{ 0xAB70, 0xABBF, "Cherokee Supplement" },
	{ 0xABC0, 0xABFF, "Meetei Mayek" },
	{ 0xAC00, 0xD7AF, "Hangul Syllables" },
	{ 0xD7B0, 0xD7FF, "Hangul Jamo Extended-B" },
	{ 0xD800, 0xDB7F, "High Surrogates" },
	{ 0xDB80, 0xDBFF, "High Private Use Surrogates" },
	{ 0xDC00, 0xDFFF, "Low Surrogates" },
	{ 0xE000, 0xF8FF, "Private Use Area" },
	{ 0xF900, 0xFAFF, "CJK Compatibility Ideographs" },
	{ 0xFB00, 0xFB4F, "Alphabetic Presentation Forms" },
	{ 0xFB50, 0xFDFF, "Arabic Presentation Forms-A" },
	{ 0xFE00, 0xFE0F, "Variation Selectors" },
	{ 0xFE10, 0xFE1F, "Vertical Forms" },
	{ 0xFE20, 0xFE2F, "Combining Half Marks" },
	{ 0xFE30, 0xFE4F, "CJK Compatibility Forms" },
	{ 0xFE50, 0xFE6F, "Small Form Variants" },
	{ 0xFE70, 0xFEFF, "Arabic Presentation Forms-B" },
	{ 0xFF00, 0xFFEF, "Halfwidth and Fullwidth Forms" },
	{ 0xFFF0, 0xFFFF, "Specials" },
	{ 0x10000, 0x1007F, "Linear B Syllabary" },
	{ 0x10080, 0x100FF, "Linear B Ideograms" },
	{ 0x10100, 0x1013F, "Aegean Numbers" },
	{ 0x10140, 0x1018F, "Ancient Greek Numbers" },
	{ 0x10190, 0x101CF, "Ancient Symbols" },
	{ 0x101D0, 0x101FF, "Phaistos Disc" },
	{ 0x10280, 0x1029F, "Lycian" },
	{ 0x102A0, 0x102DF, "Carian" },
	{ 0x102E0, 0x102FF, "Coptic Epact Numbers" },
	{ 0x10300, 0x1032F, "Old Italic" },
	{ 0x10330, 0x1034F, "Gothic" },
	{ 0x10350, 0x1037F, "Old Permic" },
	{ 0x10380, 0x1039F, "Ugaritic" },
	{ 0x103A0, 0x103DF, "Old Persian" },
	{ 0x10400, 0x1044F, "Deseret" },
	{ 0x10450, 0x1047F, "Shavian" },
	{ 0x10480, 0x104AF, "Osmanya" },
	{ 0x104B0, 0x104FF, "Osage" },
	{ 0x10500, 0x1052F, "Elbasan" },
	{ 0x10530, 0x1056F, "Caucasian Albanian" },
	{ 0x10600, 0x1077F, "Linear A" },
	{ 0x10800, 0x1083F, "Cypriot Syllabary" },
	{ 0x10840, 0x1085F, "Imperial Aramaic" },
	{ 0x10860, 0x1087F, "Palmyrene" },
	{ 0x10880, 0x108AF, "Nabataean" },
	{ 0x108E0, 0x108FF, "Hatran" },
	{ 0x10900, 0x1091F, "Phoenician" },
	{ 0x10920, 0x1093F, "Lydian" },
	{ 0x10980, 0x1099F, "Meroitic Hieroglyphs" },
	{ 0x109A0, 0x109FF, "Meroitic Cursive" },
	{ 0x10A00, 0x10A5F, "Kharoshthi" },
	{ 0x10A60, 0x10A7F, "Old South Arabian" },
	{ 0x10A80, 0x10A9F, "Old North Arabian" },
	{ 0x10AC0, 0x10AFF, "Manichaean" },
	{ 0x10B00, 0x10B3F, "Avestan" },
	{ 0x10B40, 0x10B5F, "Inscriptional Parthian" },
	{ 0x10B60, 0x10B7F, "Inscriptional Pahlavi" },
	{ 0x10B80, 0x10BAF, "Psalter Pahlavi" },
	{ 0x10C00, 0x10C4F, "Old Turkic" },
	{ 0x10C80, 0x10CFF, "Old Hungarian" },
	{ 0x10E60, 0x10E7F, "Rumi Numeral Symbols" },
	{ 0x11000, 0x1107F, "Brahmi" },
	{ 0x11080, 0x110CF, "Kaithi" },
	{ 0x110D0, 0x110FF, "Sora Sompeng" },
	{ 0x11100, 0x1114F, "Chakma" },
	{ 0x11150, 0x1117F, "Mahajani" },
	{ 0x11180, 0x111DF, "Sharada" },
	{ 0x111E0, 0x111FF, "Sinhala Archaic Numbers" },
	{ 0x11200, 0x1124F, "Khojki" },
	{ 0x11280, 0x112AF, "Multani" },
	{ 0x112B0, 0x112FF, "Khudawadi" },
	{ 0x11300, 0x1137F, "Grantha" },
	{ 0x11400, 0x1147F, "Newa" },
	{ 0x11480, 0x114DF, "Tirhuta" },
	{ 0x11580, 0x115FF, "Siddham" },
	{ 0x11600, 0x1165F, "Modi" },
	{ 0x11660, 0x1167F, "Mongolian Supplement" },
	{ 0x11680, 0x116CF, "Takri" },
	{ 0x11700, 0x1173F, "Ahom" },
	{ 0x118A0, 0x118FF, "Warang Citi" },
	{ 0x11A00, 0x11A4F, "Zanabazar Square" },
	{ 0x11A50, 0x11AAF, "Soyombo" },
	{ 0x11AC0, 0x11AFF, "Pau Cin Hau" },
	{ 0x11C00, 0x11C6F, "Bhaiksuki" },
	{ 0x11C70, 0x11CBF, "Marchen" },
	{ 0x11D00, 0x11D5F, "Masaram Gondi" },
	{ 0x12000, 0x123FF, "Cuneiform" },
	{ 0x12400, 0x1247F, "Cuneiform Numbers and Punctuation" },
	{ 0x12480, 0x1254F, "Early Dynastic Cuneiform" },
	{ 0x13000, 0x1342F, "Egyptian Hieroglyphs" },
	{ 0x14400, 0x1467F, "Anatolian Hieroglyphs" },
	{ 0x16800, 0x16A3F, "Bamum Supplement" },
	{ 0x16A40, 0x16A6F, "Mro" },
	{ 0x16AD0, 0x16AFF, "Bassa Vah" },
	{ 0x16B00, 0x16B8F, "Pahawh Hmong" },
	{ 0x16F00, 0x16F9F, "Miao" },
	{ 0x16FE0, 0x16FFF, "Ideographic Symbols and Punctuation" },
	{ 0x17000, 0x187FF, "Tangut" },
	{ 0x18800, 0x18AFF, "Tangut Components" },
	{ 0x1B000, 0x1B0FF, "Kana Supplement" },
	{ 0x1B100, 0x1B12F, "Kana Extended-A" },
	{ 0x1B170, 0x1B2FF, "Nushu" },
	{ 0x1BC00, 0x1BC9F, "Duployan" },
	{ 0x1BCA0, 0x1BCAF, "Shorthand Format Controls" },
	{ 0x1D000, 0x1D0FF, "Byzantine Musical Symbols" },
	{ 0x1D100, 0x1D1FF, "Musical Symbols" },
	{ 0x1D200, 0x1D24F, "Ancient Greek Musical Notation" },
	{ 0x1D300, 0x1D35F, "Tai Xuan Jing Symbols" },
	{ 0x1D360, 0x1D37F, "Counting Rod Numerals" },
	{ 0x1D400, 0x1D7FF, "Mathematical Alphanumeric Symbols" },
	{ 0x1D800, 0x1DAAF, "Sutton SignWriting" },
	{ 0x1E000, 0x1E02F, "Glagolitic Supplement" },
	{ 0x1E800, 0x1E8DF, "Mende Kikakui" },
	{ 0x1E900, 0x1E95F, "Adlam" },
	{ 0x1EE00, 0x1EEFF, "Arabic Mathematical Alphabetic Symbols" },
	{ 0x1F000, 0x1F02F, "Mahjong Tiles" },
	{ 0x1F030, 0x1F09F, "Domino Tiles" },
	{ 0x1F0A0, 0x1F0FF, "Playing Cards" },
	{ 0x1F100, 0x1F1FF, "Enclosed Alphanumeric Supplement" },
	{ 0x1F200, 0x1F2FF, "Enclosed Ideographic Supplement" },
	{ 0x1F300, 0x1F5FF, "Miscellaneous Symbols and Pictographs" },
	{ 0x1F600, 0x1F64F, "Emoticons" },
	{ 0x1F650, 0x1F67F, "Ornamental Dingbats" },
	{ 0x1F680, 0x1F6FF, "Transport and Map Symbols" },
	{ 0x1F700, 0x1F77F, "Alchemical Symbols" },
	{ 0x1F780, 0x1F7FF, "Geometric Shapes Extended" },
	{ 0x1F800, 0x1F8FF, "Supplemental Arrows-C" },
	{ 0x1F900, 0x1F9FF, "Supplemental Symbols and Pictographs" },
	{ 0x20000, 0x2A6DF, "CJK Unified Ideographs Extension B" },
	{ 0x2A700, 0x2B73F, "CJK Unified Ideographs Extension C" },
	{ 0x2B740, 0x2B81F, "CJK Unified Ideographs Extension D" },
	{ 0x2B820, 0x2CEAF, "CJK Unified Ideographs Extension E" },
	{ 0x2CEB0, 0x2EBEF, "CJK Unified Ideographs Extension F" },
	{ 0x2F800, 0x2FA1F, "CJK Compatibility Ideographs Supplement" },
	{ 0xE0000, 0xE007F, "Tags" },
	{ 0xE0100, 0xE01EF, "Variation Selectors Supplement" },
	{ 0xF0000, 0xFFFFF, "Supplementary Private Use Area-A" },
	{ 0x100000, 0x10FFFF, "Supplementary Private Use Area-B" },
	{ 0x110000, 0xFFFFFFFF, "No_Block" }
};

R_API const char *r_utf_block_name(int idx) {
	if (idx < 0 || idx >= lastUtfBlock) {
		return NULL;
	}
	return r_utf_blocks[idx].name;
}

#define r_utf_blocks_count (sizeof (r_utf_blocks) / sizeof (r_utf_blocks[0]))

/* Convert an UTF-8 buf into a unicode RRune */
R_API int r_utf8_decode (const ut8 *ptr, int ptrlen, RRune *ch) {
	if (ptrlen < 1) {
		return 0;
	}
	if (ptrlen > 0 && ptr[0] < 0x80) {
		if (ch) {
			*ch = (ut32)ptr[0];
		}
		return 1;
	} else if (ptrlen>1 && (ptr[0]&0xe0) == 0xc0 && (ptr[1]&0xc0) == 0x80) {
		if (ch) {
			*ch = (ptr[0] & 0x1f) << 6 | (ptr[1] & 0x3f);
		}
		return 2;
	} else if (ptrlen>2 && (ptr[0]&0xf0) == 0xe0 && (ptr[1]&0xc0) == 0x80 && (ptr[2]&0xc0) == 0x80) {
		if (ch) {
			*ch = (ptr[0] & 0xf) << 12 | (ptr[1] & 0x3f) << 6 | (ptr[2] & 0x3f);
		}
		return 3;
	} else if (ptrlen>3 && (ptr[0]&0xf8) == 0xf0 && (ptr[1]&0xc0) == 0x80 && (ptr[2]&0xc0) == 0x80 && (ptr[3]&0xc0) == 0x80) {
		if (ch) {
			*ch = (ptr[0] & 7) << 18 | (ptr[1] & 0x3f) << 12 | (ptr[2] & 0x3f) << 6 | (ptr[3] & 0x3f);
		}
		return 4;
	}
	return 0;
}

/* Convert a unicode RRune into an UTF-8 buf */
R_API int r_utf8_encode (ut8 *ptr, const RRune ch) {
	if (ch < 0x80) {
		ptr[0] = (ut8)ch;
		return 1;
	}
	else if (ch < 0x800) {
		ptr[0] = 0xc0 | (ch >> 6);
		ptr[1] = 0x80 | (ch & 0x3f);
		return 2;
	}
	else if (ch < 0x10000) {
		ptr[0] = 0xe0 | (ch >> 12);
		ptr[1] = 0x80 | ((ch >> 6) & 0x3f);
		ptr[2] = 0x80 | (ch & 0x3f);
		return 3;
	}
	else if (ch < 0x200000) {
		ptr[0] = 0xf0 | (ch >> 18);
		ptr[1] = 0x80 | ((ch >> 12) & 0x3f);
		ptr[2] = 0x80 | ((ch >> 6) & 0x3f);
		ptr[3] = 0x80 | (ch & 0x3f );
		return 4;
	}
	return 0;
}

/* Convert a unicode RRune string into an utf-8 one */
R_API int r_utf8_encode_str (const RRune *str, ut8 *dst, const int dst_length) {
	int i, pos = 0;

	if (!str || !dst) {
		return -1;
	}

	for (i = 0; i < sizeof (str) - 1 && str[i] && pos < dst_length - 1; i++) {
		pos += r_utf8_encode (&dst[pos], str[i]);
	}

	dst[pos++] = '\0';
	return pos;
}

/* Returns the size in bytes of the utf-8 encoded char */
R_API int r_utf8_size (const ut8 *ptr) {
	const int utf8_size[] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, // 0xC0-0xCF
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, // 0xD0-0xDF
		3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, // 0xE0-0xEF
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, // 0xF0-0xFF
	};
	return (ptr[0]&0x80) ? utf8_size[ptr[0]^0x80] : 1;
}

R_API int r_utf8_strlen (const ut8 *str) {
	int i, len = 0;

	for (i = 0; str[i]; i++) {
		if ((str[i] & 0xc0) != 0x80) {
			len++;
		}
	}

	return len;
}

R_API int r_isprint (const RRune c) {
	const int last = nonprintable_ranges_count;

	int low = 0;
	int hi = last - 1;

	do {
		int mid = (low + hi) >> 1;
		if (c >= nonprintable_ranges[mid].from && c <= nonprintable_ranges[mid].to) {
			return false;
		}
		if (mid < last && c > nonprintable_ranges[mid].to) {
			low = mid + 1;
		}
		if (mid < last && c < nonprintable_ranges[mid].from) {
			hi = mid - 1;
		}
	} while (low <= hi);

	return true;
}

#if __WINDOWS__
R_API char *r_utf16_to_utf8_l (const wchar_t *wc, int len) {
	if (!wc || !len || len < -1) {
		return NULL;
	}
	char *rutf8 = NULL;
	int csize;

	if ((csize = WideCharToMultiByte (CP_UTF8, 0, wc, len, NULL, 0, NULL, NULL))) {
		++csize;
		if ((rutf8 = malloc (csize))) {
			WideCharToMultiByte (CP_UTF8, 0, wc, len, rutf8, csize, NULL, NULL);
			if (len != -1) {
				rutf8[csize - 1] = '\0';
			}
		}
	}
	return rutf8;
}

R_API wchar_t *r_utf8_to_utf16_l (const char *cstring, int len) {
	if (!cstring || !len || len < -1) {
		return NULL;
	}
	wchar_t *rutf16 = NULL;
	int wcsize;

	if ((wcsize = MultiByteToWideChar (CP_UTF8, 0, cstring, len, NULL, 0))) {
		++wcsize;
		if ((rutf16 = (wchar_t *) calloc (wcsize, sizeof (wchar_t)))) {
			MultiByteToWideChar (CP_UTF8, 0, cstring, len, rutf16, wcsize);
			if (len != -1) {
				rutf16[wcsize - 1] = L'\0';
			}
		}
	}
	return rutf16;
}

R_API const char *r_acp_to_utf8_l (const ut8 *str, int len) {
	if (!str || !len || len < -1) {
		return NULL;
	}
	int wcsize;
	if ((wcsize = MultiByteToWideChar (CP_ACP, 0, str, len, NULL, 0))) {
		wchar_t *rutf16;
		++wcsize;
		if ((rutf16 = (wchar_t *) calloc (wcsize, sizeof (wchar_t)))) {
			MultiByteToWideChar (CP_ACP, 0, str, len, rutf16, wcsize);
			if (len != -1) {
				rutf16[wcsize - 1] = L'\0';
			}
			const char *ret = r_utf16_to_utf8_l (rutf16, wcsize);
			free (rutf16);
			return ret;
		}
	}
	return NULL;
}

#endif // __WINDOWS__

R_API int r_utf_block_idx (RRune ch) {
	const int last = r_utf_blocks_count;
	int low, hi, mid;

	low = 0;
	hi = last - 1;

	do {
		mid = (low + hi) >> 1;
		if (ch >= r_utf_blocks[mid].from && ch <= r_utf_blocks[mid].to) {
			return mid;
		}
		if (mid < last && ch > r_utf_blocks[mid].to) {
			low = mid + 1;
		}
		if (mid < last && ch < r_utf_blocks[mid].from) {
			hi = mid - 1;
		}
	} while (low <= hi);

	return r_utf_blocks_count - 1; /* index for "No_Block" */
}

/* str must be UTF8-encoded */
R_API int *r_utf_block_list(const ut8 *str, int len) {
	if (!str) {
		return NULL;
	}
	if (len < 0) {
		len = strlen ((const char *)str);
	}
	static bool has_block[r_utf_blocks_count] = {0};
	int *list = R_NEWS (int, len + 1);
	if (!list) {
		return NULL;
	}
	int *list_ptr = list;
	const ut8 *str_ptr = str;
	const ut8 *str_end = str + len;
	RRune ch;
	while (str_ptr < str_end) {
		int block_idx;
		int ch_bytes = r_utf8_decode (str_ptr, str_end - str_ptr, &ch);
		if (!ch_bytes) {
			block_idx = r_utf_blocks_count - 1;
			ch_bytes = 1;
		} else {
			block_idx = r_utf_block_idx (ch);
		}
		if (!has_block[block_idx]) {
			has_block[block_idx] = true;
			*list_ptr = block_idx;
			list_ptr++;
		}
		str_ptr += ch_bytes;
	}
	*list_ptr = -1;
	for (list_ptr = list; *list_ptr != -1; list_ptr++) {
		has_block[*list_ptr] = false;
	}
	return list;
}

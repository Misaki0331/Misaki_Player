#include "FastFont.h"
using namespace Core::Draw;
const unsigned char FastFont::AsciiFontBin[]={
    //2080 Bytes(1.25KB->2.03KB)
  /* 0x00  */     0xFF,
  /* 0x01  */     0xFF,
  /* 0x02  */     0xFF,
  /* 0x03  */     0xFF,
  /* 0x04  */     0xFF,
  /* 0x05  */     0xFF,
  /* 0x06  */     0xFF,
  /* 0x07  */     0xFF,
  /* 0x08  */     0xFF,
  /* 0x09  */     0xFF,
  /* 0x0A  */     0xFF,
  /* 0x0B  */     0xFF,
  /* 0x0C  */     0xFF,
  /* 0x0D  */     0xFF,
  /* 0x0E  */     0xFF,
  /* 0x0F  */     0xFF,
  /* 0x10 ▶ */    0x41, 0x46, 0x4A, 0x44, 0x53, 0x42, 0x1C, 0xFF,
  /* 0x11 ◀ */    0x18, 0x51, 0x42, 0x4A, 0x44, 0x43, 0x46, 0xFF,
  /* 0x12 “ */    0x01, 0x08, 0x50, 0x01, 0x04, 0x0B, 0x53, 0x01, 0xFF,
  /* 0x13 ” */    0x40, 0x01, 0x09, 0x10, 0x43, 0x01, 0x0C, 0x13, 0xFF,
  /* 0x14 ↑ */    0x02, 0x49, 0x02, 0x50, 0x04, 0x22, 0x69, 0x02, 0x70, 0x04, 0xFF,
  /* 0x15 ↓ */    0x40, 0x04, 0x49, 0x02, 0x12, 0x60, 0x04, 0x69, 0x02, 0x32, 0xFF,
  /* 0x16 ● */    0x50, 0x42, 0x49, 0x44, 0x4A, 0x44, 0x4B, 0x44, 0x54, 0x42, 0xFF,
  /* 0x17 ↲ */    0x44, 0x44, 0x63, 0x23, 0x59, 0x42, 0x12, 0x32, 0xFF,
  /* 0x18 ↑ */    0x42, 0x46, 0x49, 0x02, 0x10, 0x14, 0xFF,
  /* 0x19 ↓ */    0x42, 0x46, 0x69, 0x02, 0x20, 0x24, 0xFF,
  /* 0x1A → */    0x58, 0x04, 0x53, 0x42, 0x0A, 0x2A, 0xFF,
  /* 0x1B ← */    0x58, 0x04, 0x51, 0x42, 0x0A, 0x2A, 0xFF,
  /* 0x1C ≤ */    0x03, 0x0A, 0x11, 0x1A, 0x23, 0x70, 0x04, 0xFF,
  /* 0x1D ≥ */    0x01, 0x0A, 0x13, 0x1A, 0x21, 0x70, 0x04, 0xFF,
  /* 0x1E ▲ */    0x68, 0x04, 0x59, 0x41, 0x5B, 0x41, 0x4A, 0x43, 0xFF,
  /* 0x1F ▼ */    0x48, 0x04, 0x51, 0x41, 0x53, 0x41, 0x52, 0x43, 0xFF,
  /* 0x20   */    0xFF,
  /* 0x21 ! */    0x42, 0x44, 0x32, 0xFF,
  /* 0x22 " */    0x41, 0x42, 0x43, 0x42, 0xFF,
  /* 0x23 # */    0x41, 0x46, 0x43, 0x46, 0x50, 0x04, 0x60, 0x04, 0xFF,
  /* 0x24 $ */    0x42, 0x46, 0x4C, 0x23, 0x10, 0x59, 0x02, 0x24, 0x6B, 0x23, 0xFF,
  /* 0x25 % */    0x40, 0x01, 0x48, 0x01, 0x0C, 0x13, 0x1A, 0x21, 0x28, 0x6B, 0x01, 0x73, 0x01, 0xFF,
  /* 0x26 & */    0x41, 0x01, 0x48, 0x41, 0x0B, 0x12, 0x19, 0x60, 0x41, 0x71, 0x01, 0x34, 0x2B, 0x22, 0x24, 0xFF,
  /* 0x27 ' */    0x41, 0x01, 0x0A, 0x11, 0xFF,
  /* 0x28 ( */    0x03, 0x0A, 0x51, 0x42, 0x2A, 0x33, 0xFF,
  /* 0x29 ) */    0x01, 0x0A, 0x53, 0x42, 0x2A, 0x31, 0xFF,
  /* 0x2A * */    0x4A, 0x44, 0x59, 0x02, 0x10, 0x20, 0x14, 0x24, 0xFF,
  /* 0x2B + */    0x58, 0x04, 0x4A, 0x44, 0xFF,
  /* 0x2C , */    0x61, 0x01, 0x2A, 0x31, 0xFF,
  /* 0x2D - */    0x58, 0x04, 0xFF,
  /* 0x2E . */    0x69, 0x01, 0x71, 0x01, 0xFF,
  /* 0x2F / */    0x0C, 0x13, 0x1A, 0x21, 0x28, 0xFF,
  /* 0x30 0 */    0x41, 0x02, 0x48, 0x44, 0x4C, 0x44, 0x71, 0x02, 0x13, 0x1A, 0x21, 0xFF,
  /* 0x31 1 */    0x09, 0x42, 0x45, 0x71, 0x02, 0xFF,
  /* 0x32 2 */    0x08, 0x41, 0x02, 0x4C, 0x41, 0x1B, 0x22, 0x29, 0x70, 0x04, 0xFF,
  /* 0x33 3 */    0x40, 0x04, 0x0B, 0x12, 0x1B, 0x64, 0x41, 0x73, 0x22, 0x28, 0xFF,
  /* 0x34 4 */    0x60, 0x04, 0x18, 0x11, 0x0A, 0x43, 0x46, 0xFF,
  /* 0x35 5 */    0x40, 0x04, 0x48, 0x41, 0x51, 0x02, 0x5C, 0x42, 0x71, 0x02, 0x28, 0xFF,
  /* 0x36 6 */    0x42, 0x01, 0x09, 0x50, 0x43, 0x71, 0x02, 0x64, 0x41, 0x59, 0x02, 0xFF,
  /* 0x37 7 */    0x40, 0x41, 0x41, 0x03, 0x4C, 0x41, 0x1B, 0x62, 0x42, 0xFF,
  /* 0x38 8 */    0x41, 0x02, 0x4C, 0x41, 0x48, 0x41, 0x59, 0x02, 0x64, 0x41, 0x60, 0x41, 0x71, 0x02, 0xFF,
  /* 0x39 9 */    0x41, 0x02, 0x48, 0x41, 0x59, 0x02, 0x4C, 0x43, 0x2B, 0x72, 0x21, 0xFF,
  /* 0x3A : */    0x49, 0x01, 0x51, 0x01, 0x61, 0x01, 0x69, 0x01, 0xFF,
  /* 0x3B ; */    0x49, 0x01, 0x51, 0x01, 0x61, 0x01, 0x2A, 0x31, 0xFF,
  /* 0x3C < */    0x03, 0x0A, 0x11, 0x18, 0x21, 0x2A, 0x33, 0xFF,
  /* 0x3D = */    0x50, 0x04, 0x60, 0x04, 0xFF,
  /* 0x3E > */    0x01, 0x0A, 0x13, 0x1C, 0x23, 0x2A, 0x31, 0xFF,
  /* 0x3F ? */    0x08, 0x41, 0x02, 0x4C, 0x41, 0x1B, 0x22, 0x32, 0xFF,
  /* 0x40 @ */    0x08, 0x41, 0x02, 0x4C, 0x44, 0x71, 0x02, 0x60, 0x41, 0x59, 0x01, 0x62, 0x41, 0xFF,
  /* 0x41 A */    0x48, 0x45, 0x41, 0x02, 0x4C, 0x45, 0x61, 0x02, 0xFF,
  /* 0x42 B */    0x40, 0x46, 0x41, 0x02, 0x4C, 0x41, 0x59, 0x02, 0x64, 0x41, 0x71, 0x02, 0xFF,
  /* 0x43 C */    0x41, 0x02, 0x48, 0x44, 0x71, 0x02, 0x0C, 0x2C, 0xFF,
  /* 0x44 D */    0x40, 0x46, 0x41, 0x01, 0x71, 0x01, 0x0B, 0x2B, 0x54, 0x42, 0xFF,
  /* 0x45 E */    0x40, 0x46, 0x41, 0x03, 0x71, 0x03, 0x59, 0x02, 0xFF,
  /* 0x46 F */    0x40, 0x46, 0x41, 0x03, 0x59, 0x02, 0xFF,
  /* 0x47 G */    0x5A, 0x02, 0x64, 0x42, 0x71, 0x02, 0x48, 0x44, 0x41, 0x02, 0x0C, 0xFF,
  /* 0x48 H */    0x40, 0x46, 0x59, 0x02, 0x44, 0x46, 0xFF,
  /* 0x49 I */    0x41, 0x02, 0x4A, 0x44, 0x71, 0x02, 0xFF,
  /* 0x4A J */    0x42, 0x02, 0x4B, 0x44, 0x71, 0x01, 0x28, 0xFF,
  /* 0x4B K */    0x40, 0x46, 0x19, 0x12, 0x0B, 0x04, 0x22, 0x2B, 0x34, 0xFF,
  /* 0x4C L */    0x40, 0x46, 0x71, 0x03, 0xFF,
  /* 0x4D M */    0x40, 0x46, 0x09, 0x52, 0x41, 0x0B, 0x44, 0x46, 0xFF,
  /* 0x4E N */    0x40, 0x46, 0x11, 0x1A, 0x23, 0x44, 0x46, 0xFF,
  /* 0x4F O */    0x41, 0x02, 0x48, 0x44, 0x4C, 0x44, 0x71, 0x02, 0xFF,
  /* 0x50 P */    0x40, 0x46, 0x41, 0x02, 0x4C, 0x41, 0x59, 0x02, 0xFF,
  /* 0x51 Q */    0x41, 0x02, 0x48, 0x44, 0x4C, 0x43, 0x71, 0x01, 0x22, 0x2B, 0x34, 0xFF,
  /* 0x52 R */    0x40, 0x03, 0x4C, 0x41, 0x59, 0x02, 0x48, 0x45, 0x22, 0x2B, 0x34, 0xFF,
  /* 0x53 S */    0x41, 0x03, 0x48, 0x41, 0x59, 0x02, 0x64, 0x41, 0x70, 0x03, 0xFF,
  /* 0x54 T */    0x40, 0x04, 0x4A, 0x45, 0xFF,
  /* 0x55 U */    0x40, 0x45, 0x44, 0x45, 0x71, 0x02, 0xFF,
  /* 0x56 V */    0x40, 0x44, 0x29, 0x32, 0x2B, 0x44, 0x44, 0xFF,
  /* 0x57 W */    0x40, 0x45, 0x31, 0x5A, 0x42, 0x33, 0x44, 0x45, 0xFF,
  /* 0x58 X */    0x40, 0x41, 0x44, 0x41, 0x11, 0x13, 0x1A, 0x21, 0x23, 0x68, 0x41, 0x6C, 0x41, 0xFF,
  /* 0x59 Y */    0x40, 0x42, 0x44, 0x42, 0x19, 0x1B, 0x62, 0x42, 0xFF,
  /* 0x5A Z */    0x40, 0x04, 0x0C, 0x13, 0x1A, 0x21, 0x28, 0x70, 0x04, 0xFF,
  /* 0x5B [ */    0x41, 0x02, 0x49, 0x44, 0x71, 0x02, 0xFF,
  /* 0x5C \ */    0x08, 0x11, 0x1A, 0x23, 0x2C, 0xFF,
  /* 0x5D ] */    0x41, 0x02, 0x4B, 0x44, 0x71, 0x02, 0xFF,
  /* 0x5E ^ */    0x10, 0x09, 0x02, 0x0B, 0x14, 0xFF,
  /* 0x5F _ */    0x70, 0x04, 0xFF,
  /* 0x60 ` */    0x01, 0x0A, 0x13, 0xFF,
  /* 0x61 a */    0x51, 0x02, 0x5C, 0x42, 0x61, 0x02, 0x28, 0x71, 0x03, 0xFF,
  /* 0x62 b */    0x40, 0x46, 0x71, 0x02, 0x5C, 0x42, 0x53, 0x21, 0x19, 0xFF,
  /* 0x63 c */    0x51, 0x02, 0x58, 0x42, 0x71, 0x02, 0x2C, 0xFF,
  /* 0x64 d */    0x44, 0x46, 0x73, 0x22, 0x1B, 0x52, 0x21, 0x58, 0x42, 0xFF,
  /* 0x65 e */    0x61, 0x03, 0x1C, 0x53, 0x22, 0x58, 0x42, 0x71, 0x02, 0xFF,
  /* 0x66 f */    0x49, 0x45, 0x58, 0x02, 0x42, 0x01, 0x0C, 0xFF,
  /* 0x67 g */    0x4C, 0x23, 0x50, 0x41, 0x61, 0x02, 0x54, 0x43, 0x73, 0x22, 0xFF,
  /* 0x68 h */    0x40, 0x46, 0x5C, 0x43, 0x53, 0x21, 0x19, 0xFF,
  /* 0x69 i */    0x02, 0x51, 0x01, 0x5A, 0x42, 0x71, 0x02, 0xFF,
  /* 0x6A j */    0x03, 0x52, 0x01, 0x5B, 0x42, 0x72, 0x21, 0x28, 0xFF,
  /* 0x6B k */    0x40, 0x46, 0x21, 0x1A, 0x13, 0x2A, 0x33, 0xFF,
  /* 0x6C l */    0x41, 0x01, 0x4A, 0x44, 0x71, 0x02, 0xFF,
  /* 0x6D m */    0x50, 0x44, 0x11, 0x5A, 0x41, 0x13, 0x5C, 0x43, 0xFF,
  /* 0x6E n */    0x50, 0x44, 0x19, 0x52, 0x01, 0x5C, 0x43, 0xFF,
  /* 0x6F o */    0x51, 0x02, 0x58, 0x42, 0x5C, 0x42, 0x71, 0x02, 0xFF,
  /* 0x70 p */    0x50, 0x44, 0x51, 0x02, 0x61, 0x02, 0x1C, 0xFF,
  /* 0x71 q */    0x54, 0x44, 0x5B, 0x41, 0x52, 0x21, 0x18, 0x61, 0x01, 0xFF,
  /* 0x72 r */    0x50, 0x44, 0x19, 0x52, 0x01, 0x1C, 0xFF,
  /* 0x73 s */    0x53, 0x22, 0x18, 0x61, 0x02, 0x2C, 0x70, 0x03, 0xFF,
  /* 0x74 t */    0x50, 0x02, 0x41, 0x45, 0x72, 0x01, 0x2C, 0xFF,
  /* 0x75 u */    0x50, 0x43, 0x71, 0x01, 0x2B, 0x54, 0x44, 0xFF,
  /* 0x76 v */    0x50, 0x42, 0x29, 0x32, 0x2B, 0x54, 0x42, 0xFF,
  /* 0x77 w */    0x50, 0x43, 0x31, 0x62, 0x41, 0x33, 0x54, 0x43, 0xFF,
  /* 0x78 x */    0x10, 0x14, 0x19, 0x1B, 0x22, 0x29, 0x2B, 0x30, 0x34, 0xFF,
  /* 0x79 y */    0x50, 0x41, 0x54, 0x43, 0x71, 0x02, 0x61, 0x02, 0xFF,
  /* 0x7A z */    0x50, 0x04, 0x1B, 0x22, 0x29, 0x70, 0x04, 0xFF,
  /* 0x7B { */    0x03, 0x4A, 0x41, 0x19, 0x62, 0x41, 0x33, 0xFF,
  /* 0x7C | */    0x42, 0x46, 0xFF,
  /* 0x7D } */    0x01, 0x4A, 0x41, 0x1B, 0x62, 0x41, 0x31, 0xFF,
  /* 0x7E ~ */    0x20, 0x59, 0x01, 0x23, 0x1C, 0xFF,
  /* 0x7F  */    0x02, 0x09, 0x0B, 0x50, 0x44, 0x54, 0x44, 0x71, 0x02, 0xFF,
  /* 0x80 ｱﾝﾃﾅ */ 0x40, 0x04, 0x08, 0x0C, 0x51, 0x02, 0x4A, 0x45, 0xFF,
  /* 0x81 A1 */   0x60, 0x42, 0xFF,
  /* 0x82 A2 */   0x60, 0x42, 0x52, 0x44, 0xFF,
  /* 0x83 A3 */   0x60, 0x42, 0x52, 0x44, 0x44, 0x46, 0xFF,
  /* 0x84 SD */   0x40, 0x46, 0x41, 0x01, 0x0B, 0x54, 0x44, 0x71, 0x02, 0xFF,
  /* 0x85 🔔 */   0x42, 0x45, 0x60, 0x04, 0x49, 0x42, 0x4B, 0x42, 0xFF,
  /* 0x86 ｽﾋﾟｰｶｰ */0x51, 0x42, 0x52, 0x42, 0x0B, 0x04, 0x2B, 0x34, 0xFF,
  /* 0x87 S1 */   0x50, 0x42, 0xFF,
  /* 0x88 S2 */   0x50, 0x42, 0x4A, 0x44, 0xFF,
  /* 0x89 S3 */   0x50, 0x42, 0x4A, 0x44, 0x44, 0x46, 0xFF,
  /* 0x8A 🔋 */   0x02, 0x48, 0x04, 0x50, 0x44, 0x54, 0x44, 0x71, 0x02, 0xFF,
  /* 0x8B AC */   0x41, 0x41, 0x43, 0x41, 0x50, 0x04, 0x58, 0x04, 0x62, 0x42, 0x73, 0x01, 0xFF,
  /* 0x8C ♡ */    0x50, 0x42, 0x49, 0x44, 0x52, 0x44, 0x4B, 0x44, 0x54, 0x42, 0xFF,
  /* 0x8D ♪ */    0x68, 0x02, 0x70, 0x02, 0x42, 0x44, 0x0B, 0x54, 0x41, 0xFF,
  /* 0x8E ♬ */    0x42, 0x01, 0x4C, 0x45, 0x6B, 0x41, 0x13, 0x4A, 0x43, 0x61, 0x41, 0x60, 0x41, 0xFF,
  /* 0x8F ⏸ */  0x40, 0x46, 0x41, 0x46, 0x43, 0x46, 0x44, 0x46, 0xFF,
  /* 0x90 ￥ */   0x00, 0x04, 0x09, 0x0B, 0x50, 0x04, 0x60, 0x04, 0x5A, 0x43, 0xFF,
  /* 0x91 ¤ */    0x51, 0x02, 0x5B, 0x41, 0x59, 0x41, 0x22, 0x28, 0x2C, 0x0C, 0x08, 0xFF,
  /* 0x92 £ */    0x42, 0x01, 0x49, 0x44, 0x30, 0x72, 0x01, 0x2C, 0x58, 0x02, 0xFF,
  /* 0x93 ¦ */    0x42, 0x42, 0x62, 0x42, 0xFF,
  /* 0x94 ≪ */   0x0A, 0x0C, 0x11, 0x13, 0x18, 0x1A, 0x21, 0x23, 0x2A, 0x2C, 0xFF,
  /* 0x95 ≫ */   0x08, 0x0A, 0x11, 0x13, 0x1A, 0x1C, 0x21, 0x23, 0x28, 0x2A, 0xFF,
  /* 0x96 ± */    0x50, 0x04, 0x42, 0x44, 0x70, 0x04, 0xFF,
  /* 0x97 0 */    0x48, 0x42, 0x61, 0x01, 0x5B, 0x62, 0x41, 0x01, 0xFF,
  /* 0x98 1 */    0x48, 0x01, 0x41, 0x44, 0x60, 0x02, 0xFF,
  /* 0x99 2 */    0x08, 0x41, 0x01, 0x0B, 0x12, 0x19, 0x60, 0x03, 0xFF,
  /* 0x9A 3 */    0x40, 0x02, 0x0B, 0x51, 0x01, 0x1B, 0x60, 0x02, 0xFF,
  /* 0x9B § */    0x0C, 0x43, 0x21, 0x09, 0x12, 0x19, 0x1B, 0x22, 0x2B, 0x28, 0x71, 0x01, 0xFF,
  /* 0x9C Pt */   0x40, 0x45, 0x41, 0x01, 0x0B, 0x51, 0x01, 0x62, 0x02, 0x5B, 0x43, 0x34, 0xFF,
  /* 0x9D © */    0x40, 0x04, 0x4C, 0x45, 0x48, 0x45, 0x71, 0x02, 0x5B, 0x21, 0x52, 0x42, 0xFF,
  /* 0x9E ® */    0x40, 0x46, 0x41, 0x03, 0x4C, 0x45, 0x73, 0x22, 0x12, 0x2A, 0x23, 0xFF,
  /* 0x9F ‾ */    0x40, 0x04, 0xFF,
  /* 0xA0 Я */    0x44, 0x46, 0x41, 0x02, 0x48, 0x41, 0x59, 0x02, 0x22, 0x29, 0x30, 0xFF,
  /* 0xA1 。 */   0x60, 0x02, 0x6A, 0x41, 0x68, 0x41, 0x31, 0xFF,
  /* 0xA2 「 */   0x44, 0x22, 0x4A, 0x42, 0xFF,
  /* 0xA3 」 */   0x5A, 0x43, 0x70, 0x01, 0xFF,
  /* 0xA4 、 */   0x20, 0x29, 0x32, 0xFF,
  /* 0xA5 ・ */   0x59, 0x01, 0x61, 0x01, 0xFF,
  /* 0xA6 ヲ */   0x32, 0x2B, 0x64, 0x63, 0x48, 0x03, 0x58, 0x03, 0xFF,
  /* 0xA7 ァ */   0x51, 0x03, 0x1C, 0x62, 0x01, 0x2A, 0x31, 0xFF,
  /* 0xA8 ィ */   0x62, 0x42, 0x28, 0x21, 0x1A, 0x13, 0xFF,
  /* 0xA9 ゥ */   0x12, 0x58, 0x41, 0x59, 0x03, 0x64, 0x41, 0x72, 0x01, 0xFF,
  /* 0xAA ェ */   0x58, 0x04, 0x62, 0x41, 0x70, 0x04, 0xFF,
  /* 0xAB ォ */   0x58, 0x04, 0x53, 0x44, 0x22, 0x29, 0x30, 0xFF,
  /* 0xAC ャ */   0x58, 0x04, 0x24, 0x2B, 0x51, 0x44, 0xFF,
  /* 0xAD ュ */   0x59, 0x02, 0x63, 0x41, 0x70, 0x04, 0xFF,
  /* 0xAE ョ */   0x50, 0x03, 0x70, 0x03, 0x60, 0x03, 0x5B, 0x42, 0xFF,
  /* 0xAF ッ */   0x58, 0x41, 0x5A, 0x41, 0x5C, 0x42, 0x72, 0x01, 0xFF,
  /* 0xB0 ー */   0x58, 0x03, 0x24, 0xFF,
  /* 0xB1 ア */   0x40, 0x04, 0x4C, 0x41, 0x1B, 0x52, 0x43, 0x31, 0xFF,
  /* 0xB2 イ */   0x04, 0x0B, 0x52, 0x44, 0x19, 0x20, 0xFF,
  /* 0xB3 ウ */   0x02, 0x50, 0x41, 0x48, 0x04, 0x54, 0x42, 0x2B, 0x32, 0xFF,
  /* 0xB4 エ */   0x48, 0x04, 0x52, 0x43, 0x70, 0x04, 0xFF,
  /* 0xB5 オ */   0x48, 0x04, 0x43, 0x46, 0x1A, 0x21, 0x28, 0xFF,
  /* 0xB6 カ */   0x48, 0x04, 0x54, 0x43, 0x33, 0x41, 0x45, 0x30, 0xFF,
  /* 0xB7 キ */   0x48, 0x04, 0x58, 0x04, 0x42, 0x46, 0xFF,
  /* 0xB8 ク */   0x49, 0x03, 0x54, 0x42, 0x2B, 0x32, 0x11, 0x18, 0xFF,
  /* 0xB9 ケ */   0x41, 0x41, 0x4A, 0x02, 0x10, 0x53, 0x43, 0x32, 0xFF,
  /* 0xBA コ */   0x48, 0x04, 0x54, 0x43, 0x70, 0x04, 0xFF,
  /* 0xBB サ */   0x48, 0x04, 0x41, 0x42, 0x43, 0x44, 0x2A, 0x31, 0xFF,
  /* 0xBC シ */   0x40, 0x01, 0x50, 0x01, 0x4C, 0x44, 0x70, 0x03, 0xFF,
  /* 0xBD ス */   0x48, 0x04, 0x14, 0x1B, 0x22, 0x2B, 0x34, 0x29, 0x30, 0xFF,
  /* 0xBE セ */   0x48, 0x04, 0x14, 0x1B, 0x41, 0x45, 0x72, 0x02, 0xFF,
  /* 0xBF ソ */   0x40, 0x41, 0x11, 0x44, 0x44, 0x2B, 0x71, 0x01, 0xFF,
  /* 0xC0 タ */   0x49, 0x03, 0x71, 0x01, 0x11, 0x18, 0x1A, 0x54, 0x42, 0x63, 0x41, 0xFF,
  /* 0xC1 チ */   0x03, 0x4A, 0x22, 0x58, 0x04, 0x52, 0x43, 0x31, 0xFF,
  /* 0xC2 ツ */   0x48, 0x42, 0x4A, 0x42, 0x4C, 0x43, 0x2B, 0x32, 0xFF,
  /* 0xC3 テ */   0x41, 0x02, 0x50, 0x04, 0x5A, 0x42, 0x31, 0xFF,
  /* 0xC4 ト */   0x41, 0x46, 0x1A, 0x23, 0xFF,
  /* 0xC5 ナ */   0x50, 0x04, 0x42, 0x44, 0x29, 0x30, 0xFF,
  /* 0xC6 ニ */   0x49, 0x02, 0x70, 0x04, 0xFF,
  /* 0xC7 ヌ */   0x48, 0x04, 0x14, 0x1B, 0x19, 0x22, 0x29, 0x2B, 0x30, 0xFF,
  /* 0xC8 ネ */   0x02, 0x48, 0x04, 0x13, 0x1A, 0x61, 0x02, 0x28, 0x2C, 0x6A, 0x41, 0xFF,
  /* 0xC9 ノ */   0x43, 0x44, 0x2A, 0x31, 0xFF,
  /* 0xCA ハ */   0x58, 0x43, 0x0A, 0x13, 0x5C, 0x43, 0xFF,
  /* 0xCB ヒ */   0x51, 0x03, 0x40, 0x45, 0x71, 0x03, 0xFF,
  /* 0xCC フ */   0x48, 0x04, 0x54, 0x42, 0x2B, 0x71, 0x01, 0xFF,
  /* 0xCD ヘ */   0x10, 0x09, 0x12, 0x1B, 0x64, 0x41, 0xFF,
  /* 0xCE ホ */   0x48, 0x04, 0x42, 0x46, 0x60, 0x41, 0x64, 0x41, 0xFF,
  /* 0xCF マ */   0x48, 0x04, 0x54, 0x41, 0x23, 0x21, 0x2A, 0x33, 0xFF,
  /* 0xD0 ミ */   0x49, 0x02, 0x59, 0x02, 0x69, 0x02, 0x34, 0xFF,
  /* 0xD1 ム */   0x0A, 0x11, 0x58, 0x42, 0x69, 0x02, 0x64, 0x42, 0xFF,
  /* 0xD2 メ */   0x4C, 0x41, 0x19, 0x1B, 0x22, 0x29, 0x2B, 0x30, 0xFF,
  /* 0xD3 モ */   0x48, 0x04, 0x58, 0x04, 0x51, 0x43, 0x72, 0x02, 0xFF,
  /* 0xD4 ヤ */   0x50, 0x04, 0x1C, 0x23, 0x41, 0x46, 0xFF,
  /* 0xD5 ユ */   0x49, 0x02, 0x53, 0x43, 0x70, 0x04, 0xFF,
  /* 0xD6 ヨ */   0x48, 0x04, 0x54, 0x43, 0x70, 0x04, 0x58, 0x03, 0xFF,
  /* 0xD7 ラ */   0x41, 0x02, 0x50, 0x04, 0x5C, 0x41, 0x2B, 0x32, 0xFF,
  /* 0xD8 リ */   0x40, 0x43, 0x43, 0x44, 0x2A, 0x31, 0xFF,
  /* 0xD9 ル */   0x48, 0x45, 0x4A, 0x45, 0x33, 0x64, 0x41, 0xFF,
  /* 0xDA レ */   0x48, 0x45, 0x31, 0x2A, 0x23, 0x1C, 0xFF,
  /* 0xDB ロ */   0x48, 0x45, 0x49, 0x03, 0x54, 0x44, 0x71, 0x02, 0xFF,
  /* 0xDC ワ */   0x48, 0x42, 0x49, 0x03, 0x54, 0x42, 0x2B, 0x32, 0xFF,
  /* 0xDD ン */   0x48, 0x01, 0x70, 0x02, 0x2B, 0x5C, 0x41, 0xFF,
  /* 0xDE ゛ */   0x08, 0x11, 0x02, 0x0B, 0xFF,
  /* 0xDF ° */    0x40, 0x02, 0x48, 0x41, 0x51, 0x01, 0x0A, 0xFF,
  /* 0xE0 α */    0x54, 0x41, 0x63, 0x41, 0x34, 0x71, 0x01, 0x58, 0x42, 0x11, 0x1A, 0xFF,
  /* 0xE1 ä */    0x01, 0x03, 0x51, 0x02, 0x5C, 0x43, 0x61, 0x02, 0x28, 0x71, 0x02, 0xFF,
  /* 0xE2 β */    0x48, 0x45, 0x41, 0x02, 0x0C, 0x51, 0x02, 0x1C, 0x61, 0x02, 0xFF,
  /* 0xE3 ɛ */    0x51, 0x02, 0x18, 0x61, 0x01, 0x28, 0x71, 0x02, 0x2C, 0xFF,
  /* 0xE4 µ */    0x48, 0x45, 0x61, 0x01, 0x1B, 0x4C, 0x43, 0xFF,
  /* 0xE5 _ */    0x51, 0x03, 0x58, 0x42, 0x71, 0x02, 0x1A, 0x23, 0x2C, 0xFF,
  /* 0xE6 _ */    0x42, 0x01, 0x4C, 0x42, 0x09, 0x50, 0x44, 0x61, 0x02, 0xFF,
  /* 0xE7 g */    0x41, 0x03, 0x4C, 0x44, 0x73, 0x22, 0x48, 0x42, 0x61, 0x02, 0xFF,
  /* 0xE8 √ */    0x20, 0x29, 0x52, 0x42, 0x53, 0x01, 0xFF,
  /* 0xE9 -1 */   0x48, 0x01, 0x43, 0x42, 0xFF,
  /* 0xEA j */    0x03, 0x52, 0x01, 0x5B, 0x42, 0x72, 0x21, 0x28, 0xFF,
  /* 0xEB x */    0x00, 0x02, 0x09, 0x10, 0x12, 0xFF,
  /* 0xEC ¢ */    0x51, 0x02, 0x58, 0x41, 0x69, 0x02, 0x24, 0x4A, 0x45, 0xFF,
  /* 0xED 記 */   0x50, 0x02, 0x60, 0x02, 0x41, 0x46, 0x72, 0x02, 0xFF,
  /* 0xEE ñ */    0x50, 0x44, 0x19, 0x52, 0x01, 0x5C, 0x43, 0x41, 0x02, 0xFF,
  /* 0xEF ö */    0x51, 0x02, 0x58, 0x42, 0x71, 0x02, 0x5C, 0x42, 0x01, 0x03, 0xFF,
  /* 0xF0 p */    0x40, 0x46, 0x09, 0x42, 0x01, 0x4C, 0x42, 0x63, 0x22, 0xFF,
  /* 0xF1 q */    0x41, 0x01, 0x0B, 0x48, 0x42, 0x61, 0x02, 0x44, 0x46, 0xFF,
  /* 0xF2 8 */    0x49, 0x02, 0x50, 0x42, 0x69, 0x02, 0x54, 0x42, 0x59, 0x02, 0xFF,
  /* 0xF3 ∞ */    0x11, 0x58, 0x41, 0x21, 0x1A, 0x53, 0x01, 0x1C, 0x23, 0xFF,
  /* 0xF4 Ω */    0x49, 0x02, 0x50, 0x41, 0x54, 0x41, 0x61, 0x41, 0x63, 0x41, 0x28, 0x2C, 0xFF,
  /* 0xF5 ü */    0x01, 0x03, 0x50, 0x43, 0x71, 0x01, 0x2B, 0x54, 0x44, 0xFF,
  /* 0xF6 ∑ */    0x40, 0x04, 0x08, 0x11, 0x1A, 0x21, 0x28, 0x70, 0x04, 0xFF,
  /* 0xF7 π */    0x50, 0x04, 0x59, 0x41, 0x5B, 0x42, 0x2C, 0x28, 0xFF,
  /* 0xF8 x */    0x40, 0x04, 0x10, 0x14, 0x19, 0x1B, 0x22, 0x29, 0x2B, 0x30, 0x34, 0xFF,
  /* 0xF9 y */    0x40, 0x42, 0x59, 0x02, 0x44, 0x45, 0x71, 0x02, 0xFF,
  /* 0xFA 千 */   0x0C, 0x50, 0x03, 0x60, 0x04, 0x5A, 0x43, 0xFF,
  /* 0xFB 万 */   0x50, 0x04, 0x59, 0x42, 0x30, 0x62, 0x02, 0x6C, 0x41, 0xFF,
  /* 0xFC 円 */   0x50, 0x44, 0x51, 0x03, 0x5C, 0x43, 0x61, 0x02, 0x1A, 0xFF,
  /* 0xFD ÷ */    0x0A, 0x58, 0x04, 0x2A, 0xFF,
  /* 0xFE ■ */    0x48, 0x04, 0x50, 0x04, 0x58, 0x04, 0x60, 0x04, 0x68, 0x04, 0xFF,
  /* 0xFF FF */   0x40, 0x46, 0x41, 0x46, 0x42, 0x46, 0x43, 0x46, 0x44, 0x46, 0xFF
};

const unsigned short FastFont::AsciiFontPtr[] = {
0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
16,24,32,41,50,61,72,83,92,99,106,113,120,128,136,145,
154,155,159,164,173,184,198,214,219,226,233,242,247,252,255,260,
266,278,284,295,306,314,326,338,348,363,375,384,393,401,406,414,
423,437,446,459,468,479,488,495,507,514,521,529,539,544,553,561,
570,579,591,603,614,619,626,634,643,657,666,676,683,689,696,702,
705,709,719,729,737,747,757,765,776,784,792,801,809,816,825,833,
842,850,860,867,876,884,892,900,909,919,928,936,944,947,955,961,
971,980,983,988,995,1005,1014,1023,1026,1031,1038,1048,1061,1072,1082,1096,
1105,1116,1128,1139,1144,1155,1166,1173,1182,1189,1198,1207,1220,1233,1246,1258,
1261,1273,1281,1286,1291,1295,1300,1309,1317,1324,1334,1341,1349,1356,1363,1372,
1381,1385,1394,1401,1411,1418,1426,1435,1442,1451,1460,1467,1476,1485,1495,1504,
1513,1525,1534,1543,1551,1556,1563,1568,1578,1590,1595,1602,1609,1617,1624,1633,
1642,1650,1659,1668,1677,1684,1691,1700,1709,1716,1724,1731,1740,1749,1757,1762,
1770,1782,1794,1805,1815,1823,1833,1843,1854,1861,1866,1875,1881,1891,1900,1910,
1921,1931,1941,1952,1962,1975,1985,1995,2004,2016,2025,2033,2043,2053,2058,2069
};
const unsigned char FastFont::AsciiOldFont[]={
  0x7f, 0x00, 0x00, 0x00, 0x00,
  0x7f, 0x7f, 0x00, 0x00, 0x00,
  0x7f, 0x7f, 0x7f, 0x00, 0x00,
  0x7f, 0x7f, 0x7f, 0x7f, 0x00,
  0x7f, 0x7f, 0x7f, 0x7f, 0x7f,
  0x00, 0x7f, 0x7f, 0x7f, 0x7f,
  0x00, 0x00, 0x7f, 0x7f, 0x7f,
  0x00, 0x00, 0x00, 0x7f, 0x7f,
  0x00, 0x00, 0x00, 0x00, 0x7f,
  0x60, 0x70, 0x3f, 0x02, 0x3c,
  0x03, 0x03, 0x30, 0x48, 0x48,
  0x03, 0x7b, 0x28, 0x28, 0x08,
  0x04, 0x0c, 0x1c, 0x0c, 0x04,
  0x00, 0x3e, 0x1c, 0x08, 0x00,
  0x00, 0x08, 0x1c, 0x3e, 0x00,
  0x10, 0x18, 0x1c, 0x18, 0x10,
  0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x5f, 0x00, 0x00,
  0x00, 0x07, 0x00, 0x07, 0x00,
  0x14, 0x7f, 0x14, 0x7f, 0x14,
  0x24, 0x2a, 0x7f, 0x2a, 0x12,
  0x23, 0x13, 0x08, 0x64, 0x62,
  0x36, 0x49, 0x55, 0x22, 0x50,
  0x00, 0x05, 0x03, 0x00, 0x00,
  0x00, 0x1c, 0x22, 0x41, 0x00,
  0x00, 0x41, 0x22, 0x1c, 0x00,
  0x08, 0x2a, 0x1c, 0x2a, 0x08,
  0x08, 0x08, 0x3e, 0x08, 0x08,
  0x00, 0x50, 0x30, 0x00, 0x00,
  0x08, 0x08, 0x08, 0x08, 0x08,
  0x00, 0x60, 0x60, 0x00, 0x00,
  0x20, 0x10, 0x08, 0x04, 0x02,
  0x3e, 0x51, 0x49, 0x45, 0x3e,
  0x00, 0x42, 0x7f, 0x40, 0x00,
  0x42, 0x61, 0x51, 0x49, 0x46,
  0x21, 0x41, 0x45, 0x4b, 0x31,
  0x18, 0x14, 0x12, 0x7f, 0x10,
  0x27, 0x45, 0x45, 0x45, 0x39,
  0x3c, 0x4a, 0x49, 0x49, 0x30,
  0x01, 0x71, 0x09, 0x05, 0x03,
  0x36, 0x49, 0x49, 0x49, 0x36,
  0x06, 0x49, 0x49, 0x29, 0x1e,
  0x00, 0x36, 0x36, 0x00, 0x00,
  0x00, 0x56, 0x36, 0x00, 0x00,
  0x00, 0x08, 0x14, 0x22, 0x41,
  0x14, 0x14, 0x14, 0x14, 0x14,
  0x41, 0x22, 0x14, 0x08, 0x00,
  0x02, 0x01, 0x51, 0x09, 0x06,
  0x32, 0x49, 0x79, 0x41, 0x3e,
  0x7e, 0x11, 0x11, 0x11, 0x7e,
  0x7f, 0x49, 0x49, 0x49, 0x36,
  0x3e, 0x41, 0x41, 0x41, 0x22,
  0x7f, 0x41, 0x41, 0x22, 0x1c,
  0x7f, 0x49, 0x49, 0x49, 0x41,
  0x7f, 0x09, 0x09, 0x01, 0x01,
  0x3e, 0x41, 0x41, 0x51, 0x32,
  0x7f, 0x08, 0x08, 0x08, 0x7f,
  0x00, 0x41, 0x7f, 0x41, 0x00,
  0x20, 0x40, 0x41, 0x3f, 0x01,
  0x7f, 0x08, 0x14, 0x22, 0x41,
  0x7f, 0x40, 0x40, 0x40, 0x40,
  0x7f, 0x02, 0x04, 0x02, 0x7f,
  0x7f, 0x04, 0x08, 0x10, 0x7f,
  0x3e, 0x41, 0x41, 0x41, 0x3e,
  0x7f, 0x09, 0x09, 0x09, 0x06,
  0x3e, 0x41, 0x51, 0x21, 0x5e,
  0x7f, 0x09, 0x19, 0x29, 0x46,
  0x46, 0x49, 0x49, 0x49, 0x31,
  0x01, 0x01, 0x7f, 0x01, 0x01,
  0x3f, 0x40, 0x40, 0x40, 0x3f,
  0x1f, 0x20, 0x40, 0x20, 0x1f,
  0x7f, 0x20, 0x18, 0x20, 0x7f,
  0x63, 0x14, 0x08, 0x14, 0x63,
  0x03, 0x04, 0x78, 0x04, 0x03,
  0x61, 0x51, 0x49, 0x45, 0x43,
  0x00, 0x00, 0x7f, 0x41, 0x41,
  0x02, 0x04, 0x08, 0x10, 0x20,
  0x41, 0x41, 0x7f, 0x00, 0x00,
  0x04, 0x02, 0x01, 0x02, 0x04,
  0x40, 0x40, 0x40, 0x40, 0x40,
  0x00, 0x01, 0x02, 0x04, 0x00,
  0x20, 0x54, 0x54, 0x54, 0x78,
  0x7f, 0x48, 0x44, 0x44, 0x38,
  0x38, 0x44, 0x44, 0x44, 0x20,
  0x38, 0x44, 0x44, 0x48, 0x7f,
  0x38, 0x54, 0x54, 0x54, 0x18,
  0x08, 0x7e, 0x09, 0x01, 0x02,
  0x08, 0x14, 0x54, 0x54, 0x3c,
  0x7f, 0x08, 0x04, 0x04, 0x78,
  0x00, 0x44, 0x7d, 0x40, 0x00,
  0x20, 0x40, 0x44, 0x3d, 0x00,
  0x00, 0x7f, 0x10, 0x28, 0x44,
  0x00, 0x41, 0x7f, 0x40, 0x00,
  0x7c, 0x04, 0x18, 0x04, 0x78,
  0x7c, 0x08, 0x04, 0x04, 0x78,
  0x38, 0x44, 0x44, 0x44, 0x38,
  0x7c, 0x14, 0x14, 0x14, 0x08,
  0x08, 0x14, 0x14, 0x18, 0x7c,
  0x7c, 0x08, 0x04, 0x04, 0x08,
  0x48, 0x54, 0x54, 0x54, 0x20,
  0x04, 0x3f, 0x44, 0x40, 0x20,
  0x3c, 0x40, 0x40, 0x20, 0x7c,
  0x1c, 0x20, 0x40, 0x20, 0x1c,
  0x3c, 0x40, 0x30, 0x40, 0x3c,
  0x44, 0x28, 0x10, 0x28, 0x44,
  0x0c, 0x50, 0x50, 0x50, 0x3c,
  0x44, 0x64, 0x54, 0x4c, 0x44,
  0x00, 0x08, 0x36, 0x41, 0x00,
  0x00, 0x00, 0x7f, 0x00, 0x00,
  0x00, 0x41, 0x36, 0x08, 0x00,
  0x08, 0x08, 0x2a, 0x1c, 0x08,
  0x08, 0x1c, 0x2a, 0x08, 0x08,
  0x70, 0x29, 0x24, 0x29, 0x70,
  0x70, 0x28, 0x25, 0x28, 0x70,
  0x70, 0x2a, 0x25, 0x2a, 0x70,
  0x20, 0x54, 0x56, 0x55, 0x78,
  0x20, 0x54, 0x55, 0x54, 0x78,
  0x7f, 0x7f, 0x49, 0x49, 0x49,
  0x3d, 0x42, 0x42, 0x42, 0x3d,
  0x30, 0x4a, 0x48, 0x4a, 0x30,
  0x5c, 0x32, 0x2a, 0x26, 0x1d,
  0x18, 0x64, 0x3c, 0x26, 0x18,
  0x3d, 0x40, 0x40, 0x40, 0x3d,
  0x38, 0x42, 0x40, 0x22, 0x78,
  0x15, 0x16, 0x7c, 0x16, 0x15,
  0x54, 0x34, 0x1c, 0x16, 0x15,
  0x18, 0x04, 0x18, 0x20, 0x18,
  0x4a, 0x55, 0x55, 0x55, 0x29,
  0x7e, 0x09, 0x7f, 0x09, 0x09,
  0x34, 0x54, 0x38, 0x54, 0x58,
  0x48, 0x7e, 0x49, 0x49, 0x42,
  0x7f, 0x05, 0x15, 0x7a, 0x50,
  0x1c, 0x3e, 0x3e, 0x3e, 0x1c,
  0x1c, 0x22, 0x22, 0x22, 0x1c,
  0x08, 0x1c, 0x3e, 0x1c, 0x08,
  0x08, 0x14, 0x22, 0x14, 0x08,
  0x00, 0x00, 0x77, 0x00, 0x00,
  0x0e, 0x51, 0x31, 0x11, 0x11,
  0x60, 0x50, 0x48, 0x44, 0x7e,
  0x40, 0x44, 0x4a, 0x51, 0x40,
  0x40, 0x51, 0x4a, 0x44, 0x40,
  0x10, 0x38, 0x54, 0x10, 0x1f,
  0x04, 0x02, 0x7f, 0x02, 0x04,
  0x10, 0x20, 0x7f, 0x20, 0x10,
  0x00, 0x00, 0x00, 0x00, 0x00,
  0x70, 0x50, 0x70, 0x00, 0x00,
  0x00, 0x00, 0x0f, 0x01, 0x01,
  0x40, 0x40, 0x78, 0x00, 0x00,
  0x10, 0x20, 0x40, 0x00, 0x00,
  0x00, 0x30, 0x30, 0x00, 0x00,
  0x05, 0x05, 0x45, 0x25, 0x1f,
  0x00, 0x00, 0x00, 0x00, 0x00,
  0x04, 0x44, 0x34, 0x14, 0x0c,
  0x20, 0x10, 0x78, 0x04, 0x00,
  0x18, 0x08, 0x4c, 0x38, 0x00,
  0x48, 0x28, 0x18, 0x7c, 0x08,
  0x08, 0x7c, 0x08, 0x28, 0x18,
  0x40, 0x48, 0x48, 0x78, 0x40,
  0x54, 0x54, 0x54, 0x7c, 0x00,
  0x18, 0x00, 0x58, 0x40, 0x38,
  0x08, 0x08, 0x08, 0x08, 0x08,
  0x01, 0x41, 0x3d, 0x09, 0x07,
  0x20, 0x10, 0xf8, 0x04, 0x02,
  0x0e, 0x02, 0x43, 0x22, 0x1e,
  0x42, 0x42, 0x7e, 0x42, 0x42,
  0x22, 0x12, 0x0a, 0x7f, 0x02,
  0x42, 0x3f, 0x02, 0x42, 0x3e,
  0x0a, 0x0a, 0x7f, 0x0a, 0x0a,
  0x04, 0x43, 0x41, 0x21, 0x1f,
  0x04, 0x03, 0x42, 0x3e, 0x02,
  0x41, 0x41, 0x41, 0x41, 0x7f,
  0x02, 0x4f, 0x22, 0x1f, 0x02,
  0x4a, 0x4a, 0x40, 0x20, 0x1c,
  0x42, 0x22, 0x12, 0x2a, 0x46,
  0x02, 0x3f, 0x42, 0x4a, 0x46,
  0x03, 0x44, 0x40, 0x20, 0x1e,
  0x08, 0x46, 0x4a, 0x32, 0x1e,
  0x0a, 0x4a, 0x3e, 0x09, 0x08,
  0x0e, 0x00, 0x4e, 0x20, 0x1e,
  0x04, 0x45, 0x3d, 0x05, 0x04,
  0x00, 0x7f, 0x08, 0x10, 0x00,
  0x44, 0x24, 0x1f, 0x04, 0x04,
  0x40, 0x42, 0x42, 0x42, 0x40,
  0x42, 0x2a, 0x12, 0x2a, 0x06,
  0x22, 0x12, 0x7b, 0x16, 0x22,
  0x00, 0x40, 0x20, 0x1f, 0x00,
  0x78, 0x00, 0x02, 0x04, 0x78,
  0x3f, 0x44, 0x44, 0x44, 0x44,
  0x02, 0x42, 0x42, 0x22, 0x1e,
  0x04, 0x02, 0x04, 0x08, 0x30,
  0x32, 0x02, 0x7f, 0x02, 0x32,
  0x02, 0x12, 0x22, 0x52, 0x0e,
  0x00, 0x2a, 0x2a, 0x2a, 0x40,
  0x38, 0x24, 0x22, 0x20, 0x70,
  0x40, 0x28, 0x10, 0x28, 0x06,
  0x02, 0x3e, 0x4a, 0x4a, 0x4a,
  0x04, 0x7f, 0x04, 0x14, 0x0c,
  0x40, 0x42, 0x42, 0x7e, 0x40,
  0x4a, 0x4a, 0x4a, 0x4a, 0x7e,
  0x04, 0x05, 0x45, 0x25, 0x1c,
  0x0f, 0x40, 0x20, 0x1f, 0x00,
  0x7c, 0x00, 0x7e, 0x40, 0x30,
  0x7e, 0x40, 0x20, 0x10, 0x08,
  0x7e, 0x42, 0x42, 0x42, 0x7e,
  0x0e, 0x02, 0x42, 0x22, 0x1e,
  0x42, 0x42, 0x40, 0x20, 0x18,
  0x02, 0x04, 0x01, 0x02, 0x00,
  0x07, 0x05, 0x07, 0x00, 0x00,
  0x38, 0x44, 0x48, 0x30, 0x40,
  0x20, 0x55, 0x54, 0x55, 0x78,
  0x7c, 0x2a, 0x2a, 0x2a, 0x14,
  0x28, 0x54, 0x54, 0x44, 0x20,
  0x7e, 0x10, 0x10, 0x08, 0x1e,
  0x38, 0x44, 0x4c, 0x54, 0x24,
  0x78, 0x14, 0x12, 0x12, 0x0c,
  0x08, 0x54, 0x54, 0x54, 0x3c,
  0x20, 0x40, 0x3c, 0x04, 0x04,
  0x02, 0x02, 0x00, 0x07, 0x00,
  0x20, 0x40, 0x44, 0x3d, 0x00,
  0x05, 0x02, 0x05, 0x00, 0x00,
  0x18, 0x24, 0x7e, 0x24, 0x10,
  0x14, 0x7f, 0x54, 0x40, 0x40,
  0x7c, 0x09, 0x05, 0x05, 0x78,
  0x38, 0x45, 0x44, 0x45, 0x38,
  0x7e, 0x14, 0x12, 0x12, 0x0c,
  0x0c, 0x12, 0x12, 0x14, 0x7e,
  0x3c, 0x4a, 0x4a, 0x4a, 0x3c,
  0x30, 0x28, 0x10, 0x28, 0x18,
  0x58, 0x64, 0x04, 0x64, 0x58,
  0x3c, 0x41, 0x40, 0x21, 0x7c,
  0x63, 0x55, 0x49, 0x41, 0x41,
  0x44, 0x3c, 0x04, 0x7c, 0x44,
  0x45, 0x29, 0x11, 0x29, 0x45,
  0x0c, 0x50, 0x50, 0x50, 0x3c,
  0x0a, 0x0a, 0x7e, 0x0a, 0x09,
  0x44, 0x3c, 0x14, 0x14, 0x74,
  0x7c, 0x14, 0x1c, 0x14, 0x7c,
  0x08, 0x08, 0x2a, 0x08, 0x08,
  0x00, 0x00, 0x00, 0x00, 0x00,
  0x7f, 0x7f, 0x7f, 0x7f, 0x7f,

};

uint16_t FastFont::Color16[] = { //コンソールの文字色(赤5ビット 緑6ビット 青5ビットの16ビット) MS-DOSにちなんで16色に設定できるようにした。ちなみに色はMinecraftから取りました。確かコンソールモードで"|*（ここに16進数色コード）"を入力するとその地点で色変わるよ。
  0x0000, 0x0015, 0x0540, 0x0555, 0xA800, 0xA815, 0xFd40, 0xAD55,
  0x52AA, 0x52BF, 0x57EA, 0x57FF, 0xFAAA, 0xFABF, 0xFFEA, 0xFFFF
};
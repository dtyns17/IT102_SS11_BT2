#include <stdio.h>

// Dinh nghia cau truc kien hang
typedef struct {
    char ma_kien_hang[20];
    float trong_luong;      // Don vi: kg
    float nhiet_do_yeu_cau; // Don vi: do C
} PalletItem;

// Dinh nghia cau truc ke kho
typedef struct {
    char ma_ke[20];
    float trong_luong_hien_tai; // Don vi: kg
    float tai_trong_toi_da;     // Don vi: kg (co dinh 500.0 kg)
    int la_kho_lanh;            // 1: Kho lanh, 0: Kho thuong
} WarehouseShelf;

// Ham kiem tra an toan khi nhap kien hang vao ke kho
// Tra ve 1 neu hop le, tra ve 0 neu vi pham quy tac an toan
int kiem_tra_nhap_kho(WarehouseShelf ke, PalletItem kien) {
    // Kiem tra tai trong toi da
    if (ke.trong_luong_hien_tai + kien.trong_luong > ke.tai_trong_toi_da) {
        return 0; // Vi pham tai trong
    }

    // Kiem tra dieu kien kho lanh
    if (ke.la_kho_lanh == 1) {
        // SUA LOI: doi || thanh &&
        // Nhiet do hop le khi DONG THOI >= -18.0 va <= 5.0 (bao gom ca 2 moc bien)
        if (kien.nhiet_do_yeu_cau >= -18.0 && kien.nhiet_do_yeu_cau <= 5.0) {
            return 1; // Cho phep nhap kho
        } else {
            return 0; // Khong phu hop nhiet do
        }
    }

    return 1; // Hop le cho kho thuong
}

// Ham ho tro: chay 1 test case, in ket qua thuc te so voi mong doi
void chay_test(const char *ten_test, WarehouseShelf ke, PalletItem kien, int mong_doi) {
    int thuc_te = kiem_tra_nhap_kho(ke, kien);

    printf("%s\n", ten_test);
    printf("  Ke %s: hien tai %.1f kg | Kho lanh: %s\n",
           ke.ma_ke, ke.trong_luong_hien_tai, ke.la_kho_lanh ? "Co" : "Khong");
    printf("  Kien %s: %.1f kg | %.1f C\n",
           kien.ma_kien_hang, kien.trong_luong, kien.nhiet_do_yeu_cau);
    printf("  KET QUA: %s | Mong doi: %s => %s\n\n",
           thuc_te ? "CHO PHEP NHAP KHO" : "TU CHOI NHAP KHO",
           mong_doi ? "CHO PHEP NHAP KHO" : "TU CHOI NHAP KHO",
           thuc_te == mong_doi ? "DAT" : "KHONG DAT");
}

int main() {
    WarehouseShelf ke_lanh_A1 = {"KE-COLD-01", 350.0, 500.0, 1};

    printf("--- HE THONG KIEM DINH NHAP KHO WAREHOUSE ---\n\n");

    // Tinh huong goc cua de bai: hang kho 25 do C vao kho lanh -> phai TU CHOI
    PalletItem kien_hang_loi = {"PL-DRY-88", 80.0, 25.0};
    chay_test("[Tinh huong de bai] Hang kho 25 C vao kho lanh", ke_lanh_A1, kien_hang_loi, 0);

    // TH1: Kien hang hop le vao kho lanh (-10 C, 100 kg, tong 450 kg <= 500 kg)
    PalletItem kien_hop_le = {"PL-FROZEN-01", 100.0, -10.0};
    chay_test("[TH1] Kien hop le vao kho lanh", ke_lanh_A1, kien_hop_le, 1);

    // TH2a: Nhiet do qua cao (25 C)
    PalletItem kien_qua_nong = {"PL-HOT-02", 100.0, 25.0};
    chay_test("[TH2a] Nhiet do qua cao (25 C)", ke_lanh_A1, kien_qua_nong, 0);

    // TH2b: Nhiet do qua thap (-25 C)
    PalletItem kien_qua_lanh = {"PL-COLD-03", 100.0, -25.0};
    chay_test("[TH2b] Nhiet do qua thap (-25 C)", ke_lanh_A1, kien_qua_lanh, 0);

    // TH3: Vuot tai trong (350 + 200 = 550 kg > 500 kg), nhiet do hop le
    PalletItem kien_qua_nang = {"PL-HEAVY-04", 200.0, -10.0};
    chay_test("[TH3] Vuot tai trong toi da", ke_lanh_A1, kien_qua_nang, 0);

    // Bo sung: cac moc bien nhiet do va tai trong
    PalletItem kien_bien_thap = {"PL-EDGE-05", 100.0, -18.0};
    chay_test("[Bien] Nhiet do dung -18.0 C", ke_lanh_A1, kien_bien_thap, 1);

    PalletItem kien_bien_cao = {"PL-EDGE-06", 100.0, 5.0};
    chay_test("[Bien] Nhiet do dung 5.0 C", ke_lanh_A1, kien_bien_cao, 1);

    PalletItem kien_du_500 = {"PL-EDGE-07", 150.0, 0.0};
    chay_test("[Bien] Tong tai trong dung 500 kg", ke_lanh_A1, kien_du_500, 1);

    // Bo sung: kho thuong khong bi rang buoc nhiet do
    WarehouseShelf ke_thuong = {"KE-DRY-01", 100.0, 500.0, 0};
    chay_test("[Kho thuong] Hang 25 C vao kho thuong", ke_thuong, kien_hang_loi, 1);

    return 0;
}

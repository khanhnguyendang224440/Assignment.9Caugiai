#include <iostream>
#include <string>
#include <limits>

// Cau truc luu tru thong tin cua moi file
struct FileNode {
    std::string fileName;
    long fileSize;            // Kich thuoc file (tinh bang byte)
    std::string creationDate; // Ngay tao file (dang "YYYY-MM-DD")
    FileNode* next;
};

// Khoi tao danh sach trong
FileNode* head = NULL;

// Ham chen file vao danh sach theo trat tu thoi gian
void insertFileInOrder(const std::string& name, long size, const std::string& date) {
    FileNode* newNode = new FileNode;
    newNode->fileName = name;
    newNode->fileSize = size;
    newNode->creationDate = date;
    newNode->next = NULL;

    if (!head || head->creationDate > date) {
        newNode->next = head;
        head = newNode;
    } else {
        FileNode* current = head;
        while (current->next && current->next->creationDate <= date) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
}

// Ham sao chep file tu thu muc khac sang danh sach
void copyFile(const std::string& name, long size, const std::string& date) {
    std::cout << "Dang sao chep file: " << name << " co kich thuoc " << size << " byte voi ngay tao " << date << std::endl;
    insertFileInOrder(name, size, date);
}

// Ham tinh tong kich thuoc cua cac file trong danh sach
long calculateTotalSize() {
    long totalSize = 0;
    FileNode* current = head;
    while (current) {
        totalSize += current->fileSize;
        current = current->next;
    }
    return totalSize;
}

// Ham xoa cac file nho nhat de phu hop voi dung luong USB
void removeSmallestFilesForBackup(long long usbSize) {
    while (calculateTotalSize() > usbSize) {
        FileNode *smallest = head, *prevSmallest = NULL;
        FileNode *current = head, *prev = NULL;

        while (current) {
            if (current->fileSize < smallest->fileSize) {
                smallest = current;
                prevSmallest = prev;
            }
            prev = current;
            current = current->next;
        }

        // Xoa file nho nhat
        if (prevSmallest) {
            prevSmallest->next = smallest->next;
        } else {
            head = head->next;
        }
        std::cout << "Dang xoa file: " << smallest->fileName << " co kich thuoc " << smallest->fileSize << " byte\n";
        delete smallest;
    }
}

// Ham in danh sach cac file trong thu muc
void printFiles() {
    FileNode* current = head;
    std::cout << "Danh sach cac file trong thu muc:\n";
    while (current) {
        std::cout << "- " << current->fileName << ", Kich thuoc: " << current->fileSize << " byte, Ngay tao: " << current->creationDate << std::endl;
        current = current->next;
    }
}

// Ham yeu cau nguoi dung nhap thong tin file
void inputFileData() {
    std::string name, date;
    long size;

    std::cout << "Nhap ten file: ";
    std::cin >> name;
    std::cout << "Nhap kich thuoc file (tinh bang byte): ";
    std::cin >> size;
    std::cout << "Nhap ngay tao (YYYY-MM-DD): ";
    std::cin >> date;

    insertFileInOrder(name, size, date);
}

int main() {
    int choice;
    long long usbSize = 32LL * 1024 * 1024 * 1024; // 32GB

    do {
        std::cout << "\nMenu:\n";
        std::cout << "1. Them moi file\n";
        std::cout << "2. Sao chep file vao thu muc\n";
        std::cout << "3. Tinh tong kich thuoc cua cac file\n";
        std::cout << "4. In danh sach cac file\n";
        std::cout << "5. Sao luu file len USB (xoa file nho nhat neu can)\n";
        std::cout << "0. Thoat\n";
        std::cout << "Nhap lua chon cua ban: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                inputFileData();
                break;
            case 2: {
                std::string name, date;
                long size;

                std::cout << "Nhap ten file can sao chep: ";
                std::cin >> name;
                std::cout << "Nhap kich thuoc file (tinh bang byte): ";
                std::cin >> size;
                std::cout << "Nhap ngay tao (YYYY-MM-DD): ";
                std::cin >> date;

                copyFile(name, size, date);
                break;
            }
            case 3: {
                long totalSize = calculateTotalSize();
                std::cout << "Tong kich thuoc cac file: " << totalSize << " byte\n";
                break;
            }
            case 4:
                printFiles();
                break;
            case 5: {
                if (calculateTotalSize() > usbSize) {
                    std::cout << "Tong kich thuoc vuot qua dung luong USB. Xoa file de thich hop.\n";
                    removeSmallestFilesForBackup(usbSize);
                }
                printFiles();
                break;
            }
            case 0:
                std::cout << "Thoat chuong trinh.\n";
                break;
            default:
                std::cout << "Lua chon khong hop le. Vui long thu lai.\n";
        }
    } while (choice != 0);

    return 0;
}


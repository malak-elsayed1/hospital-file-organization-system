#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstring>
using namespace std;


struct PrimaryIndex {
    int id, offset;
    bool operator<(const PrimaryIndex& pi) const {
        return id < pi.id;
    }
};

struct SecondaryIndex {
    char firstname[14];
    int id;
    bool operator<(const SecondaryIndex& other) const {
        return strcmp(firstname, other.firstname) < 0;
    }
    
};


vector<PrimaryIndex>  docPrimaryList;
vector<SecondaryIndex> docSecondaryList;

void saveDocIndexes() {
    ofstream pOut("DoctorPrimaryIndex.dat", ios::binary | ios::trunc);
    for (int i = 0; i < docPrimaryList.size(); i++) pOut.write((char*)&docPrimaryList[i], sizeof(PrimaryIndex));
    pOut.close();

    ofstream sOut("DoctorSecondaryIndex.dat", ios::binary | ios::trunc);
    for (int i = 0; i < (int)docSecondaryList.size(); i++) sOut.write((char*)&docSecondaryList[i], sizeof(SecondaryIndex));
    sOut.close();
}

void loadDocIndexes() {
    docPrimaryList.clear();
    docSecondaryList.clear();

    ifstream pIn("DoctorPrimaryIndex.dat", ios::binary);
    PrimaryIndex pi;
    while (pIn.read((char*)&pi, sizeof(pi))) {
        docPrimaryList.push_back(pi);
    }
    pIn.close();

    ifstream sIn("DoctorSecondaryIndex.dat", ios::binary);
    SecondaryIndex si;
    while (sIn.read((char*)&si, sizeof(si))) {
        docSecondaryList.push_back(si);
    }
    sIn.close();

    sort(docPrimaryList.begin(), docPrimaryList.end());
    sort(docSecondaryList.begin(), docSecondaryList.end());
}

int searchDocPrimary(int id) {
    for (int i = 0; i < docPrimaryList.size(); i++) {
        if (docPrimaryList[i].id == id) {
            return docPrimaryList[i].offset;
        }
    }
    return -1;
}

int searchDocSecondary(const char* name) {
    for (int i = 0; i < docSecondaryList.size(); i++) {
        if (strcmp(docSecondaryList[i].firstname, name) == 0) {
            return docSecondaryList[i].id;
        }
    }
    return -1;
}

void removeDocPrimary(int id) {
    for (int i = 0; i < docPrimaryList.size(); i++)
        if (docPrimaryList[i].id == id) { docPrimaryList.erase(docPrimaryList.begin() + i); break; }
}

void removeDocSecondary(const char* name) {
    for (int i = 0; i < docSecondaryList.size(); i++)
        if (strcmp(docSecondaryList[i].firstname, name) == 0) { docSecondaryList.erase(docSecondaryList.begin() + i); break; }
}


vector<PrimaryIndex>  patPrimaryList;
vector<SecondaryIndex> patSecondaryList;

void savePatIndexes() {
    ofstream pOut("PatientPrimaryIndex.dat", ios::binary | ios::trunc);
    for (int i = 0; i < patPrimaryList.size(); i++) {
        pOut.write((char*)&patPrimaryList[i], sizeof(PrimaryIndex));
    }    pOut.close();

    ofstream sOut("PatientSecondaryIndex.dat", ios::binary | ios::trunc);
    for (int i = 0; i < patSecondaryList.size(); i++) {
        sOut.write((char*)&patSecondaryList[i], sizeof(SecondaryIndex));
    }    sOut.close();
}

void loadPatIndexes() {
    patPrimaryList.clear();
    patSecondaryList.clear();

    ifstream pIn("PatientPrimaryIndex.dat", ios::binary);
    PrimaryIndex pi;
    while (pIn.read((char*)&pi, sizeof(pi))) patPrimaryList.push_back(pi);
    pIn.close();

    ifstream sIn("PatientSecondaryIndex.dat", ios::binary);
    SecondaryIndex si;
    while (sIn.read((char*)&si, sizeof(si))) patSecondaryList.push_back(si);
    sIn.close();

    sort(patPrimaryList.begin(), patPrimaryList.end());
    sort(patSecondaryList.begin(), patSecondaryList.end());
}

int searchPatPrimary(int id) {
    for (int i = 0; i < patPrimaryList.size(); i++) {
        if (patPrimaryList[i].id == id) {
            return patPrimaryList[i].offset;
        }
    }
    return -1;
}

int searchPatSecondary(const char* name) {
    for (int i = 0; i < patSecondaryList.size(); i++) {
        if (strcmp(patSecondaryList[i].firstname, name) == 0) {
            return patSecondaryList[i].id;
        }
    }
    return -1;
}

void removePatPrimary(int id) {
    for (int i = 0; i < patPrimaryList.size(); i++)
        if (patPrimaryList[i].id == id) {
            patPrimaryList.erase(patPrimaryList.begin() + i);
            break;
        }
}

void removePatSecondary(const char* name) {
    for (int i = 0; i < patSecondaryList.size(); i++)
        if (strcmp(patSecondaryList[i].firstname, name) == 0) {
            patSecondaryList.erase(patSecondaryList.begin() + i);
            break;
        }
}


struct Doctor {
    int id;
    char firstname[14];
    char lastname[14];
    char specialization[20];
    int yearsofexp;

    void solveGetLineProblem() {
        if (cin.peek() == '\n') cin.ignore();
    }

    void readDoctor() {
        cout << "Enter Doctor ID: "; cin >> id;
        cout << "Enter First Name: "; solveGetLineProblem(); cin.getline(firstname, 14);
        cout << "Enter Last Name: ";  solveGetLineProblem(); cin.getline(lastname, 14);
        cout << "Enter Specialization: "; solveGetLineProblem(); cin.getline(specialization, 20);
        cout << "Enter Years of Experience: "; cin >> yearsofexp;
    }

    void displayDoctor() const {
        cout << "Doctor ID          : " << id << "\n";
        cout << "First Name         : " << firstname << "\n";
        cout << "Last Name          : " << lastname << "\n";
        cout << "Specialization     : " << specialization << "\n";
        cout << "Years of Experience: " << yearsofexp << "\n";
    }

    short calcRecSize() {
        return (short)((sizeof(int) * 2) + strlen(firstname) + strlen(lastname) + strlen(specialization) + 4);
    }
};

void writeDoctorToFile(fstream& f, Doctor& dr) {
    f.write((char*)&dr.id, sizeof(dr.id));
    f.write(dr.firstname, strlen(dr.firstname)); f.put('|');
    f.write(dr.lastname, strlen(dr.lastname));   f.put('|');
    f.write(dr.specialization, strlen(dr.specialization)); f.put('|');
    f.write((char*)&dr.yearsofexp, sizeof(dr.yearsofexp));
    f.put('$');
}

void readDoctorFromFile(fstream& f, Doctor& dr) {
    f.read((char*)&dr.id, sizeof(dr.id));
    if (f.fail()) return;
    f.getline(dr.firstname, 14, '|');
    f.getline(dr.lastname, 14, '|');
    f.getline(dr.specialization, 20, '|');
    f.read((char*)&dr.yearsofexp, sizeof(dr.yearsofexp));
    f.ignore(1, '$');
}

void addDoctor(fstream& f, Doctor& dr) {
    f.clear(); f.seekg(0);
    short currentOffset;
    f.read((char*)&currentOffset, sizeof(short));

    short prevOffset = -1, bestOffset = -1;
    short newSize = dr.calcRecSize();
    short oldSize;
    short nextOffset;

    while (currentOffset != -1) {
        f.seekg(currentOffset + sizeof(int) + 1);
        f.read((char*)&nextOffset, sizeof(short));
        f.ignore(1, '|');
        f.read((char*)&oldSize, sizeof(short));

        if (oldSize >= newSize) {
            bestOffset = currentOffset;

            if (prevOffset == -1) { f.seekp(0); f.write((char*)&nextOffset, sizeof(short)); }
            else { f.seekp(prevOffset + sizeof(int) + 1); f.write((char*)&nextOffset, sizeof(short)); }
            break;
        }
        prevOffset = currentOffset;
        currentOffset = nextOffset;
    }


    if (bestOffset != -1) {

        f.seekp(bestOffset);
        writeDoctorToFile(f, dr);
        int padding = oldSize - newSize;
        while (padding) {
            f.put('#');
            padding--;
        }
        cout << "Space reclaimed successfully.\n";
    }
    else {
        f.clear(); f.seekp(0, ios::end);
        bestOffset = (int)f.tellp();
        writeDoctorToFile(f, dr);
        cout << "Doctor added to end of file.\n";
    }

    docPrimaryList.push_back({ dr.id, bestOffset });
    SecondaryIndex si; strcpy_s(si.firstname, dr.firstname); si.id = dr.id;
    docSecondaryList.push_back(si);
    sort(docPrimaryList.begin(), docPrimaryList.end());
    sort(docSecondaryList.begin(), docSecondaryList.end());
}

void searchDoctorById(fstream& f, int id) {
    int offset = searchDocPrimary(id);
    if (offset == -1) { cout << "Doctor not found.\n"; return; }
    f.clear(); f.seekg(offset);
    Doctor dr; readDoctorFromFile(f, dr);
    dr.displayDoctor();
}

void searchDoctorByName(fstream& f, const string& name) {
    int id = searchDocSecondary(name.c_str());
    if (id == -1) { cout << "Doctor not found.\n"; return; }
    searchDoctorById(f, id);
}

void deleteDoctorById(fstream& f, int id) {
    int offset = searchDocPrimary(id);
    if (offset == -1) { cout << "Doctor not found.\n"; return; }

    short oldHeader; f.clear(); f.seekg(0); f.read((char*)&oldHeader, sizeof(short));
    f.seekg(offset);
    Doctor dr; readDoctorFromFile(f, dr);
    short recSize = dr.calcRecSize();

    removeDocPrimary(id);
    removeDocSecondary(dr.firstname);

    f.seekp(offset + sizeof(int));
    f.put('*');
    f.write((char*)&oldHeader, sizeof(short));
    f.put('|');
    f.write((char*)&recSize, sizeof(short));
    f.put('$');
    f.seekp(0);
    short newH = offset;
    f.write((char*)&newH, sizeof(short));
    cout << "Doctor deleted successfully.\n";
}

void deleteDoctorByName(fstream& f, const string& name) {
    int id = searchDocSecondary(name.c_str());
    if (id == -1) { cout << "Doctor not found.\n"; return; }
    deleteDoctorById(f, id);
}

void updateDoctorById(fstream& f, int id) {
    int origOffset = searchDocPrimary(id);
    if (origOffset == -1) { cout << "Doctor not found.\n"; return; }

    f.clear(); f.seekg(origOffset);
    Doctor oldDr; readDoctorFromFile(f, oldDr);
    int oldSize = oldDr.calcRecSize();
    string oldName = oldDr.firstname;

    cout << "Current Data:\n"; oldDr.displayDoctor();
    cout << "\nEnter New Data:\n";
    Doctor newDr; newDr.readDoctor();
    int newSize = newDr.calcRecSize();

    if (newSize <= oldSize) {
        f.seekp(origOffset);
        writeDoctorToFile(f, newDr);
        int rem = oldSize - newSize;
        if (rem >= 15) {
            short oldHeader; f.seekg(0); f.read((char*)&oldHeader, sizeof(short));
            short fragPos = (short)(origOffset + newSize);
            f.seekp(fragPos);
            int dummy = -1; f.write((char*)&dummy, sizeof(int));
            f.put('*'); f.write((char*)&oldHeader, sizeof(short));
            f.put('|'); short fs = (short)rem; f.write((char*)&fs, sizeof(short)); f.put('$');
            f.seekp(0); f.write((char*)&fragPos, sizeof(short));
        }
        else { while (rem-- > 0) f.put('#'); }

        removeDocSecondary(oldName.c_str());
        SecondaryIndex si; strcpy_s(si.firstname, newDr.firstname); si.id = newDr.id;
        docSecondaryList.push_back(si);
        sort(docSecondaryList.begin(), docSecondaryList.end());

        removeDocPrimary(id);
        docPrimaryList.push_back({ newDr.id, origOffset });
        sort(docPrimaryList.begin(), docPrimaryList.end());
        cout << "Doctor updated in-place.\n";
    }
    else {
        deleteDoctorById(f, id);
        addDoctor(f, newDr);
        cout << "Doctor updated (moved to new location).\n";
    }
}

void updateDoctorByName(fstream& f, const string& name) {
    int id = searchDocSecondary(name.c_str());
    if (id == -1) { cout << "Doctor not found.\n"; return; }
    updateDoctorById(f, id);
}

void displayAllDoctors(fstream& f) {
    if (docPrimaryList.empty()) {
        cout << "No doctor records found.\n";
        return;
    }
    Doctor dr;
    cout << "\n====== All Doctors ======\n";
    for (int i = 0; i < docPrimaryList.size(); i++) {
        int offset = docPrimaryList[i].offset;
        f.seekg(offset);
        readDoctorFromFile(f, dr);
        dr.displayDoctor();

        cout << "------------------------------\n";
    }
}

void displayTop5Doctors(fstream& f) {
    if (docPrimaryList.empty()) {
        cout << "No doctor records found.\n";
        return;
    }
    Doctor dr;
    int count;
    if (docPrimaryList.size() >= 5) {
        count = 5;
    }
    else {
        cout << "Note: Only " << docPrimaryList.size() << " records found.\n";
        count = docPrimaryList.size();
    }

    for (int i = 0; i < count; i++) {
        int offset = docPrimaryList[i].offset;
        f.clear();
        f.seekg(offset);
        readDoctorFromFile(f, dr);
        dr.displayDoctor();
        cout << "--------------------------------\n";
    }
}


struct Patient {
    int id;
    char firstname[14];
    char lastname[14];
    char condition[20];
    int age;

    void solveGetLineProblem() {
        if (cin.peek() == '\n') cin.ignore();
    }

    void readPatient() {
        cout << "Enter Patient ID: "; cin >> id;
        cout << "Enter First Name: "; solveGetLineProblem(); cin.getline(firstname, 14);
        cout << "Enter Last Name: ";  solveGetLineProblem(); cin.getline(lastname, 14);
        cout << "Enter Condition: ";  solveGetLineProblem(); cin.getline(condition, 20);
        cout << "Enter Age: "; cin >> age;
    }

    void displayPatient() const {
        cout << "Patient ID : " << id << "\n";
        cout << "First Name : " << firstname << "\n";
        cout << "Last Name  : " << lastname << "\n";
        cout << "Condition  : " << condition << "\n";
        cout << "Age        : " << age << "\n";
    }

    short calcRecSize() {
        return (short)((sizeof(int) * 2) + strlen(firstname) + strlen(lastname) + strlen(condition) + 4);
    }
};

void writePatientToFile(fstream& f, Patient& pt) {
    f.write((char*)&pt.id, sizeof(pt.id));
    f.write(pt.firstname, strlen(pt.firstname)); f.put('|');
    f.write(pt.lastname, strlen(pt.lastname));   f.put('|');
    f.write(pt.condition, strlen(pt.condition)); f.put('|');
    f.write((char*)&pt.age, sizeof(pt.age));
    f.put('$');
}

void readPatientFromFile(fstream& f, Patient& pt) {
    f.read((char*)&pt.id, sizeof(pt.id));
    if (f.fail()) return;
    f.getline(pt.firstname, 14, '|');
    f.getline(pt.lastname, 14, '|');
    f.getline(pt.condition, 20, '|');
    f.read((char*)&pt.age, sizeof(pt.age));
    f.ignore(1, '$');
}

void addPatient(fstream& f, Patient& pt) {
    f.clear(); f.seekg(0);
    short currentOffset; f.read((char*)&currentOffset, sizeof(short));

    short prevOffset = -1, bestOffset = -1;
    short newSize = pt.calcRecSize();
    short oldSize;

    while (currentOffset != -1) {
        f.seekg(currentOffset + sizeof(int) + 1);
        short nextOffset; f.read((char*)&nextOffset, sizeof(short));
        f.ignore(1, '|');
        f.read((char*)&oldSize, sizeof(short));

        if (oldSize >= newSize) {
            bestOffset = currentOffset;

            if (prevOffset == -1) { f.seekp(0); f.write((char*)&nextOffset, sizeof(short)); }
            else { f.seekp(prevOffset + sizeof(int) + 1); f.write((char*)&nextOffset, sizeof(short)); }
            break;
        }
        prevOffset = currentOffset;
        currentOffset = nextOffset;
    }


    if (bestOffset != -1) {

        f.seekp(bestOffset);
        writePatientToFile(f, pt);
        int padding = oldSize - newSize;
        while (padding) {
            f.put('#');
            padding--;
        }
        cout << "Space reclaimed successfully.\n";
    }
    else {
        f.clear(); f.seekp(0, ios::end);
        bestOffset = (int)f.tellp();
        writePatientToFile(f, pt);
        cout << "Patient added to end of file.\n";
    }

    patPrimaryList.push_back({ pt.id, bestOffset });
    SecondaryIndex si; strcpy_s(si.firstname, pt.firstname); si.id = pt.id;
    patSecondaryList.push_back(si);
    sort(patPrimaryList.begin(), patPrimaryList.end());
    sort(patSecondaryList.begin(), patSecondaryList.end());
}

void searchPatientById(fstream& f, int id) {
    int offset = searchPatPrimary(id);
    if (offset == -1) { cout << "Patient not found.\n"; return; }
    f.clear(); f.seekg(offset);
    Patient pt; readPatientFromFile(f, pt);
    pt.displayPatient();
}

void searchPatientByName(fstream& f, const string& name) {
    int id = searchPatSecondary(name.c_str());
    if (id == -1) { cout << "Patient not found.\n"; return; }
    searchPatientById(f, id);
}

void deletePatientById(fstream& f, int id) {
    int offset = searchPatPrimary(id);
    if (offset == -1) { cout << "Patient not found.\n"; return; }

    short oldHeader; f.clear(); f.seekg(0); f.read((char*)&oldHeader, sizeof(short));
    f.seekg(offset);
    Patient pt; readPatientFromFile(f, pt);
    short recSize = pt.calcRecSize();

    removePatPrimary(id);
    removePatSecondary(pt.firstname);

    f.seekp(offset + sizeof(int));
    f.put('*');
    f.write((char*)&oldHeader, sizeof(short));
    f.put('|');
    f.write((char*)&recSize, sizeof(short));
    f.put('$');
    f.seekp(0);
    short newH = (short)offset;
    f.write((char*)&newH, sizeof(short));
    cout << "Patient deleted successfully.\n";
}

void deletePatientByName(fstream& f, const string& name) {
    int id = searchPatSecondary(name.c_str());
    if (id == -1) { cout << "Patient not found.\n"; return; }
    deletePatientById(f, id);
}
void updatePatientById(fstream& f, int id) {
    int origOffset = searchPatPrimary(id);
    if (origOffset == -1) { cout << "Patient not found.\n"; return; }

    f.clear(); f.seekg(origOffset);
    Patient oldPa; readPatientFromFile(f, oldPa);
    int oldSize = oldPa.calcRecSize();
    string oldName = oldPa.firstname;

    cout << "Current Data:\n"; oldPa.displayPatient();
    cout << "\nEnter New Data:\n";
    Patient newPa; newPa.readPatient();
    int newSize = newPa.calcRecSize();

    if (newSize <= oldSize) {
        f.seekp(origOffset);
        writePatientToFile(f, newPa);
        int rem = oldSize - newSize;
        if (rem >= 15) {
            short oldHeader; f.seekg(0); f.read((char*)&oldHeader, sizeof(short));
            short fragPos = (short)(origOffset + newSize);
            f.seekp(fragPos);
            int dummy = -1; f.write((char*)&dummy, sizeof(int));
            f.put('|'); f.write((char*)&oldHeader, sizeof(short));
            f.put('|'); short fs = (short)rem; f.write((char*)&fs, sizeof(short)); f.put('$');
            f.seekp(0); f.write((char*)&fragPos, sizeof(short));
        }
        else { while (rem-- > 0) f.put('#'); }

        removePatSecondary(oldName.c_str());
        SecondaryIndex si; strcpy_s(si.firstname, newPa.firstname); si.id = newPa.id;
        patSecondaryList.push_back(si);
        sort(patSecondaryList.begin(), patSecondaryList.end());

        removePatPrimary(id);
        patPrimaryList.push_back({ newPa.id, origOffset });
        sort(patPrimaryList.begin(), patPrimaryList.end());
        cout << "Patient updated in-place.\n";
    }
    else {
        deletePatientById(f, id);
        addPatient(f, newPa);
        cout << "Patient updated (moved to new location).\n";
    }
}

void updatePatientByName(fstream& f, const string& name) {
    int id = searchPatSecondary(name.c_str());
    if (id == -1) { cout << "Patient not found.\n"; return; }
    updatePatientById(f, id);
}

void displayAllPatients(fstream& f) {
    if (patPrimaryList.empty()) {
        cout << "No patient records found.\n";
        return;
    }
    Patient pt;
    cout << "\n====== All Patients ======\n";
    for (int i = 0; i < patPrimaryList.size(); i++) {
        int offset = patPrimaryList[i].offset;
        f.seekg(offset);
        readPatientFromFile(f, pt);
        pt.displayPatient();
        cout << "--------------------------\n";
    }
}

void displayTop5Patients(fstream& f) {
    if (patPrimaryList.empty()) {
        cout << "No patient records found.\n";
        return;
    }
    Patient pt;
    int count;
    if (patPrimaryList.size() >= 5) {
        count = 5;
    }
    else {
        cout << "Note: Only " << patPrimaryList.size() << " patient records are found.\n";
        count = patPrimaryList.size();
    }
    for (int i = 0; i < count; i++) {
        int offset = patPrimaryList[i].offset;
        f.clear();
        f.seekg(patPrimaryList[i].offset);
        readPatientFromFile(f, pt);
        pt.displayPatient();
        cout << "----------------------------\n";
    }
}

void initFile(const char* filename) {
    ifstream check(filename, ios::binary);
    if (!check.good()) {
        ofstream create(filename, ios::binary);
        short header = -1;
        create.write((char*)&header, sizeof(header));
        create.close();
        cout << "Created new file: " << filename << "\n";
    }
    check.close();
}


void doctorMenu(fstream& docFile) {
    int choice, id;
    string name;
    do {
        cout << "\n---------- Doctor Menu ----------\n";
        cout << "1. Add Doctor\n";
        cout << "2. Update Doctor by ID\n";
        cout << "3. Update Doctor by Name\n";
        cout << "4. Delete Doctor by ID\n";
        cout << "5. Delete Doctor by Name\n";
        cout << "6. Search Doctor by ID\n";
        cout << "7. Search Doctor by Name\n";
        cout << "8. Display All Doctors\n";
        cout << "9. Display Top 5 Doctors\n";
        cout << "0. Back to Main Menu\n";
        cout << "Choice: ";
        cin >> choice;

        switch (choice) {
        case 1: { Doctor dr; dr.readDoctor(); addDoctor(docFile, dr); break; }
        case 2: cout << "ID: "; cin >> id; updateDoctorById(docFile, id); break;
        case 3: cout << "Name: "; cin >> name; updateDoctorByName(docFile, name); break;
        case 4: cout << "ID: "; cin >> id; deleteDoctorById(docFile, id); break;
        case 5: cout << "Name: "; cin >> name; deleteDoctorByName(docFile, name); break;
        case 6: cout << "ID: "; cin >> id; searchDoctorById(docFile, id); break;
        case 7: cout << "Name: "; cin >> name; searchDoctorByName(docFile, name); break;
        case 8: displayAllDoctors(docFile); break;
        case 9: displayTop5Doctors(docFile); break;
        case 0: break;
        default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}


void patientMenu(fstream& patFile) {
    int choice, id;
    string name;
    do {
        cout << "\n---------- Patient Menu ----------\n";
        cout << "1. Add Patient\n";
        cout << "2. Update Patient by ID\n";
        cout << "3. Update Patient by Name\n";
        cout << "4. Delete Patient by ID\n";
        cout << "5. Delete Patient by Name\n";
        cout << "6. Search Patient by ID\n";
        cout << "7. Search Patient by Name\n";
        cout << "8. Display All Patients\n";
        cout << "9. Display Top 5 Patients\n";
        cout << "0. Back to Main Menu\n";
        cout << "Choice: ";
        cin >> choice;

        switch (choice) {
        case 1: { Patient pt; pt.readPatient(); addPatient(patFile, pt); break; }
        case 2: cout << "ID: "; cin >> id; updatePatientById(patFile, id); break;
        case 3: cout << "Name: "; cin >> name; updatePatientByName(patFile, name); break;
        case 4: cout << "ID: "; cin >> id; deletePatientById(patFile, id); break;
        case 5: cout << "Name: "; cin >> name; deletePatientByName(patFile, name); break;
        case 6: cout << "ID: "; cin >> id; searchPatientById(patFile, id); break;
        case 7: cout << "Name: "; cin >> name; searchPatientByName(patFile, name); break;
        case 8: displayAllPatients(patFile); break;
        case 9: displayTop5Patients(patFile); break;
        case 0: break;
        default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}


int main() {
    initFile("Doctors.dat");
    initFile("Patients.dat");

    fstream docFile("Doctors.dat", ios::in | ios::out | ios::binary);
    fstream patFile("Patients.dat", ios::in | ios::out | ios::binary);

    if (!docFile) { cerr << "Error opening Doctors.dat\n"; return 1; }
    if (!patFile) { cerr << "Error opening Patients.dat\n"; return 1; }

    loadDocIndexes();
    loadPatIndexes();

    int choice;
    do {
        cout << "\n-------------------------------------\n";
        cout << "   Hospital Management System\n";
        cout << "-------------------------------------\n";
        cout << "1. Manage Doctors\n";
        cout << "2. Manage Patients\n";
        cout << "0. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        switch (choice) {
        case 1: doctorMenu(docFile); break;
        case 2: patientMenu(patFile); break;
        case 0: cout << "Saving indexes and exiting...\n"; break;
        default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);

    saveDocIndexes();
    savePatIndexes();

    docFile.close();
    patFile.close();

    cout << "Goodbye!\n";
    return 0;
}

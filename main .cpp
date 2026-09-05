#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include <climits>
using namespace std;

string toLower(string s) {
    for (size_t i = 0; i < s.size(); i++)
        s[i] = tolower((unsigned char)s[i]);
    return s;
}
//trim
string trim(string s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    size_t b = s.find_last_not_of(" \t\r\n");
    if (a == string::npos) return "";
    return s.substr(a, b - a + 1);
}
//levesthenin
int editDistance(const string& a, const string& b) {
    int n = a.size(), m = b.size();
    vector<vector<int>> dp(n + 1, vector<int>(m + 1));
    for (int i = 0; i <= n; i++) dp[i][0] = i;
    for (int j = 0; j <= m; j++) dp[0][j] = j;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (tolower(a[i-1]) == tolower(b[j-1]))
                dp[i][j] = dp[i-1][j-1];
            else
                dp[i][j] = 1 + min({dp[i-1][j], dp[i][j-1], dp[i-1][j-1]});
        }
    }
    return dp[n][m];
}
//strng to int
int safeStoi(const string& s, int fallback) {
    string t = trim(s);
    if (t.empty()) return fallback;
    for (size_t i = 0; i < t.size(); i++)
        if (!isdigit((unsigned char)t[i])) return fallback;
    return stoi(t);
}

int allowedDist(const string& word) {
    int len = word.size();
    if (len <= 3) return 1;
    if (len <= 6) return 2;
    return 3;
}
//storing format
vector<string> splitCSV(const string& line) {
    vector<string> parts;
    string cur;
    bool inQuotes = false;
    for (size_t i = 0; i < line.size(); i++) {
        char c = line[i];
        if (c == '"') {
            inQuotes = !inQuotes;
        } else if (c == ',' && !inQuotes) {
            parts.push_back(trim(cur));
            cur.clear();
        } else if (c != '\r') {
            cur += c;
        }
    }
    parts.push_back(trim(cur));
    return parts;
}

class Doctor {
public:
    int id;
    string name, speciality, qualification, dutyTime, hospital;
    int visitFee;

    Doctor() {
        id = 0;
        name = "";
        speciality = "";
        qualification = "";
        dutyTime = "";
        hospital = "";
        visitFee = 0;

    }

    Doctor(int i, string n, string s, string q, string d, string h, int f) {
        this->id = i;
        this->name = n;
        this->speciality = s;
        this->qualification = q;
        this->dutyTime = d;
        this->hospital = h;
        this->visitFee = f;
    }

    void display() const {
        cout << "Doctor Name       : " << name << endl;
        cout << "Speciality        : " << speciality << endl;
        cout << "Qualification     : " << qualification << endl;
        cout << "Duty Time         : " << dutyTime << endl;
        cout << "Hospital/Clinic   : " << hospital << endl;
        cout << "Visit Fee         : " << visitFee << " Taka" << endl;
        cout << "----------------------------------------" << endl;
    }
};

class Surgery {
public:
    string name;
    int costMin, costMax;

    Surgery() {
        name = "";
        costMin =0;
        costMax = 0;
    }

    Surgery(string n ,int mn ,int mx ) {
        this->name = n;
        this->costMin = mn;
        this->costMax = mx;

    }
    void display() const {
        cout << name;
        for (int i = name.size(); i < 35; i++) cout << " ";
        cout << ": " << costMin << " - " << costMax << " Taka (approx.)" << endl;
    }
};

class DiseaseCategory {
public:
    string diseaseName;
    vector<string> keywords;
    vector<int> doctorIds;   

    DiseaseCategory(string dn, vector<string> kw, vector<int> ids) {
        this->diseaseName=dn;
        this->keywords = kw;
        this->doctorIds = ids;

    }
};

class User {
public:
    string username, password, fullName, phone;

    User() {
        username = "";
        password = "";
        fullName = "";
        phone = "";

    }
    User(string u, string p, string f, string ph) {
        this->username = u;
        this->password = p;
        this->fullName = f;
        this->phone = ph;

    }
};

class Appointment {
public:
    int serial;
    string patientName, username, doctorName, hospital, preferredDate, preferredTime, phone;

    Appointment() {
        serial = 0;
    }
    Appointment(int s, string pn, string un, string dn, string h, string pd, string pt, string ph) {
        patientName = pn;
        username = un;
        doctorName = dn;
        hospital = h;
        preferredDate = pd;
        preferredTime = pt;
        phone = ph;

    }
};

class ReviveSystem {
private:
    vector<Doctor> doctors;
    vector<User> users;
    vector<Appointment> appointments;
    vector<DiseaseCategory> categories;
    vector<Surgery> surgeries;
    User* currentUser;
    int nextSerial;

    
    void loadDoctors() {
        ifstream file("doctors.csv");
        if (!file.is_open()) {
            cout << "ERROR: doctors.csv not found!" << endl;
            return;
        }
        string line;
        getline(file, line); 
        while (getline(file, line)) {
            if (trim(line).empty()) continue;
            vector<string> p = splitCSV(line);
            if (p.size() < 7) continue;
            int id = safeStoi(p[0], -1);
            if (id == -1) continue;
            Doctor d(id, p[1], p[2], p[3], p[4], p[5], safeStoi(p[6], 900));
            doctors.push_back(d);
        }
        file.close();
        cout << "Here " << doctors.size() << "Doctors are ready to serve you ." << endl;
    }

    void loadUsers() {
        ifstream file("users.csv");
        if (!file.is_open()) {
            cout << "ERROR: users.csv not found!" << endl;
            return;
        }
        string line;
        getline(file, line);
        while (getline(file, line)) {
            if (trim(line).empty()) continue;
            vector<string> p = splitCSV(line);
            if (p.size() < 4) continue;
            users.push_back(User(p[0], p[1], p[2], p[3]));
        }
        file.close();

    }

    void loadAppointments() {
        ifstream file("appointments.csv");
        if (!file.is_open()) return;
        string line;
        getline(file, line);
        while (getline(file, line)) {
            if (trim(line).empty()) continue;
            vector<string> p = splitCSV(line);
            if (p.size() < 7) continue;
            string ph = (p.size() >= 8) ? p[7] : "";
            Appointment a(safeStoi(p[0], nextSerial), p[1], p[2], p[3], p[4], p[5], p[6], ph);
            appointments.push_back(a);
            if (a.serial >= nextSerial) nextSerial = a.serial + 1;
        }
        file.close();

    }

    void saveAppointments() {
        ofstream file("appointments.csv");
        file << "Serial,PatientName,Username,DoctorName,Hospital,PreferredDate,PreferredTime,Phone"<<endl;
        for (size_t i = 0; i < appointments.size(); i++) {
            Appointment& a = appointments[i];
            file << a.serial << "," << a.patientName << "," << a.username << ","
                 << a.doctorName << "," << a.hospital << ","
                 << a.preferredDate << "," << a.preferredTime << ","
                 << a.phone << "\n";
        }
        file.close();
    }

    void saveUser(const User& u) {
        ofstream file("users.csv", ios::app);
        file << u.username << "," << u.password << ","
             << u.fullName << "," << u.phone << endl;
        file.close();
    }

    void setupCategories() {
        categories.push_back(DiseaseCategory(
            "Heart Disease / High BP / Chest Pain",
            {"heart","chest pain","chestpain","heart pain","palpitation","palpitations",
             "high bp","bp","blood pressure","heart attack","cardiac","cardio"}, {12, 24}));

        categories.push_back(DiseaseCategory(
            "Migraine / Headache / Neurological Disorder",
            {"brain","headache","head ache","migraine","migren","dizziness","vertigo",
             "seizure","epilepsy","stroke","memory loss","neuro"}, {14, 30}));

        categories.push_back(DiseaseCategory(
            "Fracture / Arthritis / Joint Pain",
            {"bone","joint","fracture","back pain","backpain","knee pain","knee",
             "arthritis","sprain","ortho","orthopedic"}, {3, 15, 28, 36}));

        categories.push_back(DiseaseCategory(
            "Viral Fever / Common Cold / Flu",
            {"fever","cold","cough","flu","body pain","bodyache","weakness",
             "general","common cold"}, {1, 5, 13, 17, 25, 29}));

        categories.push_back(DiseaseCategory(
            "Eye Infection / Vision Problem / Cataract",
            {"eye","eyes","vision","blurry vision","red eye","eye pain",
             "cataract","glaucoma","ophthalmology"}, {7, 19, 40}));

        categories.push_back(DiseaseCategory(
            "ENT Infection / Sinus / Tonsillitis",
            {"ear","nose","throat","sore throat","tonsillitis","sinus",
             "hearing","ear pain","ent"}, {11, 23}));

        categories.push_back(DiseaseCategory(
            "Skin Allergy / Eczema / Acne",
            {"skin","rash","itching","allergy","acne","eczema",
             "psoriasis","dermatology","dermato"}, {42}));

        categories.push_back(DiseaseCategory(
            "Gastritis / Ulcer / Diarrhea",
            {"stomach","gastric","acidity","diarrhea","diarrhoea","vomiting",
             "constipation","ulcer","gastro","abdominal pain","belly pain"}, {43}));

        categories.push_back(DiseaseCategory(
            "Asthma / Pneumonia / Respiratory Infection",
            {"lung","lungs","breathing","asthma","shortness of breath","chest infection",
             "pneumonia","pulmonary","respiratory"}, {44}));

        categories.push_back(DiseaseCategory(
            "Kidney Stone / UTI / Kidney Disease",
            {"kidney","urine","urinary","burning urine","kidney stone","dialysis",
             "nephro","renal"}, {6, 18, 39}));

        categories.push_back(DiseaseCategory(
            "Toothache / Gum Disease / Cavity",
            {"tooth","teeth","dental","toothache","gum","cavity"}, {45}));

        categories.push_back(DiseaseCategory(
            "Menstrual Problem / PCOS / Gynecological Issue",
            {"gynecology","gynaecology","period","menstrual","pregnancy","pcos",
             "female","women health"}, {2, 10, 16, 22, 27, 35}));

        categories.push_back(DiseaseCategory(
            "Child related illness",
            {"child","baby","pediatric","paediatric","kids","infant"}, {4, 8, 20, 26, 32, 37, 41}));

        categories.push_back(DiseaseCategory(
            "Depression / Anxiety / Stress related",
            {"depression","anxiety","stress","mental","psychiatry","insomnia",
             "sleep problem"}, {46}));
    }

    void setupSurgeries() {
        surgeries.push_back(Surgery("Appendectomy (Appendix)", 25000, 45000));
        surgeries.push_back(Surgery("Cesarean Section (C-Section)", 30000, 60000));
        surgeries.push_back(Surgery("Cataract Surgery", 15000, 40000));
        surgeries.push_back(Surgery("Gallbladder Removal", 35000, 70000));
        surgeries.push_back(Surgery("Hernia Repair", 20000, 45000));
        surgeries.push_back(Surgery("Kidney Stone Removal", 30000, 80000));
        surgeries.push_back(Surgery("Knee Replacement", 150000, 350000));
        surgeries.push_back(Surgery("Heart Bypass (CABG)", 300000, 700000));
        surgeries.push_back(Surgery("Tonsillectomy", 15000, 30000));
        surgeries.push_back(Surgery("Dental Implant / Extraction", 3000, 25000));
    }

    Doctor* findDoctorById(int id) {
        for (size_t i = 0; i < doctors.size(); i++)
            if (doctors[i].id == id) return &doctors[i];
        return nullptr;
    }

public:
    ReviveSystem() : currentUser(nullptr), nextSerial(0001) {
        loadDoctors();
        loadUsers();
        loadAppointments();
        setupCategories();
        setupSurgeries();
    }

    
    bool login() {
        cout << "\n========== LOGIN ==========" << endl;
        cout << "1. Login" << endl;
        cout << "2. Register( If you are new user )" << endl;
        cout << "3. Exit" << endl;
        cout << "Choose (1/2/3): ";
        string choice;
        getline(cin, choice);
        choice = trim(choice);

        if (choice == "3") return false;
        if (choice == "2") return registerUser();

        cout << "Username: ";
        string uname;
        getline(cin, uname);
        uname = trim(uname);

        cout << "Password: ";
        string pass;
        getline(cin, pass);

        for (size_t i = 0; i < users.size(); i++) {
            if (users[i].username == uname && users[i].password == pass) {
                currentUser = &users[i];
                cout << endl<<"Welcome, " << currentUser->fullName << "!" << endl;
                return true;
            }
        }
        cout << "Wrong username or password. Try again." << endl;
        return login();
    }

    bool registerUser() {
        cout <<endl<< "----- New User Registration -----" << endl;
        cout << "Choose Username: ";
        string uname;
        getline(cin, uname);
        uname = trim(uname);

        for (size_t i = 0; i < users.size(); i++) {
            if (users[i].username == uname) {
                cout << "Username already exists!" << endl;
                return login();
            }
        }

        cout << "Choose Password: ";
        string pass;
        getline(cin, pass);

        cout << "Full Name: ";
        string fname;
        getline(cin, fname);

        cout << "Phone: ";
        string phone;
        getline(cin, phone);

        User u(uname, pass, trim(fname), trim(phone));
        users.push_back(u);
        saveUser(u);
        currentUser = &users.back();
        cout << endl<<"Registered successfully! Welcome, " << currentUser->fullName << "!" << endl;
        return true;
    }

    
    int matchDisease(const string& input, string& matchedKeyword, int& matchedDist) {
        string dis = toLower(trim(input));
        int bestCat = -1;
        int bestDist = INT_MAX;
        string bestWord;

        for (size_t c = 0; c < categories.size(); c++) {
            for (size_t k = 0; k < categories[c].keywords.size(); k++) {
                int dist = editDistance(dis, categories[c].keywords[k]);
                if (dist < bestDist) {
                    bestDist = dist;
                    bestCat = (int)c;
                    bestWord = categories[c].keywords[k];
                }
            }
        }

        matchedKeyword = bestWord;
        matchedDist = bestDist;

        if (bestCat == -1) return -1;
        if (bestDist > allowedDist(bestWord)) return -1;
        return bestCat;
    }

    void recommendDoctors(const string& input) {
        string matchedKeyword;
        int dist;
        int catIndex = matchDisease(input, matchedKeyword, dist);

        cout << endl <<"------- Recommended Doctors --------" << endl;

        if (catIndex == -1) {
            cout << "SORRY! No matching doctor found for: " << input << endl;
            cout << "Type 'help' to see the spelling list.\n" << endl;
            return;
        }

        if (dist > 0)
            cout << "(Maybe you are trying to say: \"" << matchedKeyword << "\")" << endl;

        DiseaseCategory& cat = categories[catIndex];
        cout << "Possible Disease: " << cat.diseaseName << endl;

        vector<Doctor*> matched;
        for (size_t i = 0; i < cat.doctorIds.size(); i++) {
            Doctor* d = findDoctorById(cat.doctorIds[i]);
            if (d) {
                cout << matched.size() + 1 << ") ";
                d->display();
                matched.push_back(d);
            }
        }

        if (matched.empty()) {
            cout << "No doctors found for this category." << endl;
            return;
        }

        cout << "Enter the number of the Doctor you want to book (0 to skip): ";
        string ans;
        getline(cin, ans);
        ans = trim(ans);

        int choice = safeStoi(ans, 0);
        if (choice >= 1 && choice <= (int)matched.size()) {
            bookAppointment(*matched[choice - 1]);
        } else if (choice != 0) {
            cout << "Invalid choice. Skipping booking.\n" << endl;
        }
    }

    
    bool isSlotTaken(const string& doctorName, const string& pdate, const string& ptime) {
        for (size_t i = 0; i < appointments.size(); i++) {
            if (toLower(appointments[i].doctorName) == toLower(doctorName) &&
                toLower(appointments[i].preferredDate) == toLower(pdate) &&
                toLower(appointments[i].preferredTime) == toLower(ptime)) {
                return true;
            }
        }
        return false;
    }

    void bookAppointment(Doctor& d) {
        cout << "\n----- Appointment Booking -----" << endl;
        cout << "Enter your NAME: ";
        string pname;
        getline(cin, pname);
        pname = trim(pname);
        if (pname.empty()) pname = currentUser->fullName;

        string pdate, ptime;
        while (true) {
            cout << "Preferred Date (e.g. 25-08-2026): ";
            getline(cin, pdate);
            pdate = trim(pdate);

            cout << "Preferred Time (e.g. 5:00 PM): ";
            getline(cin, ptime);
            ptime = trim(ptime);

            if (isSlotTaken(d.name, pdate, ptime)) {
                cout << "\nSorry, " << d.name << " is already booked on " << pdate
                     << " at " << ptime << ". Please choose a different date or time.\n" << endl;
            } else {
                break;
            }
        }

        Appointment a(nextSerial++, pname, currentUser->username,
                      d.name, d.hospital, pdate, ptime, currentUser->phone);
        appointments.push_back(a);
        saveAppointments();

        cout << endl<<"Appointment Confirmed!" << endl;
        cout << "Serial No     : " << a.serial << endl;
        cout << "Patient       : " << a.patientName << endl;
        cout << "Phone         : " << a.phone << endl;
        cout << "Doctor        : " << a.doctorName << endl;
        cout << "Hospital      : " << a.hospital << endl;
        cout << "Date          : " << a.preferredDate << endl;
        cout << "Time          : " << a.preferredTime << endl;
        cout << "----------------------------------------" << endl;
    }

    void bookByDoctorName(const string& input) {
        string q = toLower(trim(input));
        Doctor* best = nullptr;
        int bestDist = INT_MAX;

        for (size_t i = 0; i < doctors.size(); i++) {
            int dist = editDistance(q, doctors[i].name);
            if (dist < bestDist) {
                bestDist = dist;
                best = &doctors[i];
            }
        }

        if (best && bestDist <= (int)best->name.size() / 2) {
            cout << "\nMatched Doctor: " << best->name << endl;
            bookAppointment(*best);
        } else {
            cout << endl<<"Doctor Not Found. (First enter symptom, then book the recommended doctor)"<< endl<< endl;
        }
    }

    void showMyAppointments() {
        bool found = false;
        cout << endl<<"========== YOUR APPOINTMENTS ==========" << endl;
        for (size_t i = 0; i < appointments.size(); i++) {
            if (appointments[i].username == currentUser->username) {
                Appointment& a = appointments[i];
                cout << "Serial " << a.serial << " | " << a.patientName
                     << " (" << a.phone << ") -> " << a.doctorName << " (" << a.hospital
                     << ") on " << a.preferredDate << " " << a.preferredTime << endl;
                found = true;
            }
        }
        if (!found) cout << "No Appointment Available yet" << endl;
        cout << endl;
    }



    
    void showSurgeryCosts() {
        cout << "\n========== SURGERY COST (Approximate, Private Hospital, BD) ==========" << endl;
        for (size_t i = 0; i < surgeries.size(); i++)
            surgeries[i].display();
        cout << "Note: Actual cost depends on hospital, doctor and patient condition . its for only giving you the idea about cost range" << endl;
    }

    void showAmbulance() {
        cout << "\n========== AMBULANCE / EMERGENCY SERVICE (Bangladesh) ==========" << endl;
        cout << "National Emergency Service (Police / Fire / Ambulance) : 999" << endl;
        cout << "Islamic Bank Hospital Emergency Number : 01635-600385" << endl << endl;
        cout << "Islamic Bank Hospital Hotline Number : 01810-000123 , 01810-000118 , 01992-346631 " << endl;
        cout << "Islamic Bank Hospital Location : Plot-31,main road-03,block D,Section 11,Mirpur,Dhaka" << endl << endl;

    }

    void showVocabulary() {
        cout << "\n===== Symptom / Disease Vocabulary =====" << endl;
        for (size_t c = 0; c < categories.size(); c++) {
            cout << "\n-- " << categories[c].diseaseName << " --" << endl;
            for (size_t k = 0; k < categories[c].keywords.size(); k++)
                cout << "  " << categories[c].keywords[k] << endl;
        }

    }
};

int main() {
    ReviveSystem system;

    cout << "==================================================" << endl;
    cout << "  WELCOME TO REVIVE" << endl;
    cout << "==================================================" << endl;


    if (!system.login()) {
        cout << "Good Helth is the best gift from God . Take care of it ! Goodbye!" << endl;
        return 0;
    }

    vector<string> commands = {
        "help", "vocab", "list", "exit",
        "surgery", "surgery cost",
        "ambulance", "emergency",
        "appointment", "book appointment",
        "myappointments", "my appointments",
        "bydoctor"
    };

    while (true) {
        cout << endl<<"Commands: surgery | ambulance | emergency | appointment | myappointments | exit" <<endl;
        cout << "Enter Disease / Symptom / Command: ";
        string input;
        getline(cin, input);
        string clean = trim(input);
        if (clean.empty()) continue;

        string lower = toLower(clean);

        
        int bestDist = INT_MAX;
        string bestCmd;
        for (size_t i = 0; i < commands.size(); i++) {
            int d = editDistance(lower, commands[i]);
            if (d < bestDist) {
                bestDist = d;
                bestCmd = commands[i];
            }
        }

        bool isCommand = (bestDist <= allowedDist(bestCmd));

        if (isCommand) {
            if (bestCmd == "exit") {
                cout << "Good Helth is the best gift from God . Take care of it !! Thank you!" << endl;
                break;
            }
            else if (bestCmd == "help" || bestCmd == "vocab" || bestCmd == "list") {
                system.showVocabulary();
            }
            else if (bestCmd == "surgery" || bestCmd == "surgery cost") {
                system.showSurgeryCosts();
            }
            else if (bestCmd == "ambulance" || bestCmd == "emergency") {
                system.showAmbulance();
            }
            else if (bestCmd == "appointment" || bestCmd == "book appointment") {
                cout << "Enter the Doctor name: ";
                string dname;
                getline(cin, dname);
                system.bookByDoctorName(dname);
            }
            else if (bestCmd == "myappointments" || bestCmd == "my appointments") {
                system.showMyAppointments();
            }

            continue;
        }

        
        system.recommendDoctors(clean);
    }

    return 0;
}

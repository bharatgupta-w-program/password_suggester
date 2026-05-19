#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <numeric>
#include <algorithm>

using namespace std;



const int DIM = 32; 
const string LOWER   = "abcdefghijklmnopqrstuvwxyz";
const string UPPER   = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const string DIGITS  = "0123456789";
const string SYMBOLS = "!@#$%^&*()-_=+";
const string CHARSET = LOWER + UPPER + DIGITS + SYMBOLS; 

double dot(const vector<double>& a, const vector<double>& b) {
    double res = 0.0;
    for (size_t i = 0; i < a.size(); i++) res += a[i] * b[i];
    return res;
}

double magnitude(const vector<double>& v) {
    return sqrt(dot(v, v));
}

vector<double> norm(vector<double> v) {
    double m = magnitude(v);
    if (m > 1e-9) {
        for (auto& val : v) val /= m;
    }
    return v;
}


vector<double> strToVec(const string& s) {
    vector<double> v(DIM);
    for (size_t j = 0; j < s.size(); j++)
        for (int i = 0; i < DIM; i++)
            v[i] += s[j] * sin((i+1) * (j+1) * 0.31415926);
    return v;
}
string vecTostr(const vector<double>& e2, int length) {
    // Step 1: rank each component (argsort)
    vector<int> sorted_idx(DIM);
    iota(sorted_idx.begin(), sorted_idx.end(), 0);
    sort(sorted_idx.begin(), sorted_idx.end(),
         [&](int a, int b){ return e2[a] < e2[b]; });
 
    // Step 2: rank_of[i] = rank of component i among all 32
    vector<int> rank_of(DIM);
    for (int i = 0; i < DIM; i++)
        rank_of[sorted_idx[i]] = i;
 
    // Step 3: map rank → character index (evenly spread across charset)
    string pw;
    pw.reserve(length);
    for (int i = 0; i < length; i++) {
        int rank   = rank_of[i % DIM];
        int char_i = (int)round((double)rank / (DIM - 1)
                                * (double)(CHARSET.size() - 1));
        pw += CHARSET[char_i];
    }
    return pw;
}



 
string generatePassword(const string& username,
                        const string& masterPassword,
                        int length) {
 
    if (username.empty() || masterPassword.empty())
        return "ERROR: username and master password cannot be empty.";
 
    if (length < 8 || length > DIM) {
        cout << "   Length clamped to 8-32 range.\n";
        length = max(8, min(DIM, length));
    }
 
    // convert to vectors in R^DIM
    vector<double> u = strToVec(username);
    vector<double> m = strToVec(masterPassword);
 
    //  Gram-Schmidt
    vector<double> e1 = norm(u);
    double c1 = dot(m, e1);
 
    vector<double> w(DIM);
    for (int i = 0; i < DIM; i++)
        w[i] = m[i] - c1 * e1[i];
 
    double mag_w = magnitude(w);
 
    // Edge case: username and master password are identical (parallel vectors)
    if (mag_w < 1e-9) {
        cout << "  [!] Username and master password are too similar.\n";
        cout << "      Try a different master password.\n";
        return "";
    }
 
    vector<double> e2 = norm(w);
 
    return vecTostr(e2, length);
}


void cmdGenerate() {
    string username, masterPw;
    int length;
 
    cout << "  Username       : "; cin >> username;
    cout << "  Master Password(8-32): "; cin >> masterPw;
    // cout << "  Length (8-32)  : "; cin >> length;
    length=masterPw.size();
 
    cout << "\n";
    string pw = generatePassword(username, masterPw, length);
    if (pw.empty() || pw.substr(0,5) == "ERROR") {
        cout << "  " << pw << "\n";
        return;
    }
 
   
    cout << "  Generated Password:\n";
    cout << "      " << pw << "\n\n";
 
   
    
}
 
int main(){
  cmdGenerate();
  return 0;
}

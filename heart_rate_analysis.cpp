#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;


/* The EKG class is designed to analyze EKG data, detect peak points, classify these points, 
and save the results to files. The class has various data members to store the name of the file
to be analyzed, time and voltage data, peak points, and classification results. The name 
variable stores the file name, the times and voltages vectors store the EKG data, and 
the peakTimes and peakVoltages vectors store the peak points. Additionally, normal_T and 
normal_V store data for the normal class, bradycardia_T and bradycardia_V store data 
for the bradycardia class, and tachycardia_T and tachycardia_V store data for the tachycardia class.
The class is designed to perform all these operations in an organized manner and save 
the results in the appropriate format.*/

class EKG{
    private:
        string name;
        vector<double> zamanlar;
        vector<double> voltajlar;
        string dosyaAdi;
        vector<double> pikZamanlar;
        vector<double> pikVoltajlar;
        vector<double> normal_T;
        vector<double> bradikardi_T;
        vector<double> tasikardi_T;
        vector<double> normal_V;
        vector<double> bradikardi_V;
        vector<double> tasikardi_V;
    public:
    
        /*  The analizEt function takes the given file name and starts the EKG analysis process. 
        It reads the data from the file, calculates the peak points, classifies these points, 
        and writes the results to files. It is the core operation that sequentially performs 
        all stages of the analysis process.*/
        
        void analizEt(string isim){
            name = isim;
            dosyadanVeriOku();
            pikHesaplama();
            sınıflandırma();
            dosyaYazdırma();
        }

    
        /*  The dosyadanVeriOku function reads EKG data from the specified file and stores 
        it in the times and voltages vectors. The file is opened, and the first line (header)
        is skipped. Then, each time and voltage value in the file is read sequentially and added 
        to the corresponding vectors. After successfully reading the file, it is closed. If 
        the file cannot be opened, an error message is displayed, and the process is terminated.*/
        
        void dosyadanVeriOku() {

            ifstream dosyaOku(name+".txt");
            if (!dosyaOku.is_open()) {
                cout << "Dosya acilamadi!" << endl;
                return;
            }

            string satir;
            getline(dosyaOku, satir);
            double zaman, voltaj;

            while (dosyaOku >> zaman >> voltaj){
                zamanlar.push_back(zaman);
                voltajlar.push_back(voltaj);
            }

            dosyaOku.close();
        }

        /*  The pikHesaplama function analyzes the voltage values in the EKG data to detect and record
        peak points. It evaluates each voltage value as a peak candidate if it meets a certain threshold
        (≥ 0.06) and is compared with its neighboring values. To verify whether a candidate is a true peak,
        the next 500 values are checked; if no higher voltage is found among them, the candidate is 
        confirmed as a peak. The detected peak points' time and voltage values are stored in the peakTimes
        and peakVoltages vectors. To prevent recalculating nearby peaks, the loop counter is increased by 500.
        This process is performed to identify significant peak points in the EKG data and prepare them for further analysis.*/
        
        void pikHesaplama() {

            pikZamanlar.push_back(0);
            pikVoltajlar.push_back(0);

            for(size_t i = 1; i < voltajlar.size()-1; ++i){
                if(voltajlar[i] >= 0.06 && voltajlar[i] > voltajlar[i-1] && voltajlar[i] > voltajlar[i+1]){
                    bool result = true;
                    for(int j = 1; j <= 500; j++){
                        if(voltajlar[i] < voltajlar[i+j]){
                            result = false;
                            break;
                        }
                    }
                    if(result){
                        pikZamanlar.push_back(zamanlar[i]);
                        pikVoltajlar.push_back(voltajlar[i]);
                        i = i + 500;
                    }
                }
            }
        }

    
        /*  The sınıflandırma function categorizes peak points as normal, bradycardia, or tachycardia based on time
        and voltage differences. Peaks with a voltage difference greater than 0.0033 are classified based on their 
        time difference. If the time difference is less than 0.6 seconds, the peak is classified as tachycardia. 
        If the time difference is greater than 1 second, it is classified as bradycardia. Peaks with a time difference
        between these values are classified as normal. The classification results are stored in the corresponding vectors 
        and organized for EKG analysis.*/
        
        void sınıflandırma(){

            double zamanFark = 0, voltajFark = 0;

            for(size_t i = 1; i < pikZamanlar.size(); ++i){
                voltajFark = abs(pikVoltajlar[i] - pikVoltajlar[i-1]);
                if(voltajFark > 0.0033){
                    zamanFark = abs(pikZamanlar[i] - pikZamanlar[i+1]);
                    if(zamanFark <= 0.6) {
                        tasikardi_T.push_back(pikZamanlar[i]);
                        tasikardi_V.push_back(pikVoltajlar[i]);
                    }
                   else if(zamanFark > 1) {
                       bradikardi_T.push_back(pikZamanlar[i]);
                       bradikardi_V.push_back(pikVoltajlar[i]);
                   }
                   else {
                       normal_T.push_back(pikZamanlar[i]);
                       normal_V.push_back(pikVoltajlar[i]);
                   }
                }
                else{
                    zamanFark = abs(pikZamanlar[i] - pikZamanlar[i-1]);
                    if(zamanFark <= 0.6){
                        tasikardi_T.push_back(pikZamanlar[i]);
                        tasikardi_V.push_back(pikVoltajlar[i]);
                    }
                    else if(zamanFark > 1){
                        bradikardi_T.push_back(pikZamanlar[i]);
                        bradikardi_V.push_back(pikVoltajlar[i]);
                    }
                    else{
                        normal_T.push_back(pikZamanlar[i]);
                        normal_V.push_back(pikVoltajlar[i]);
                    }
                }
            }

        }

    
        /*  The dosyaYazdırma function writes the classified peak points to the corresponding files. Three separate
        files are opened for the normal, bradycardia, and tachycardia categories. Each file includes a header line 
        ("ZAMAN" and "VOLTAJ") and a table formatter. Then, the corresponding time (normal_T, bradikardi_T, tasikardi_T)
        and voltage (normal_V, bradikardi_V, tasikardi_V) values are written sequentially. During the writing process,
        the time and voltage values are properly aligned, and precision is adjusted. Once the process is complete, 
        the function terminates. This function is designed to systematically save the analysis results to files.*/
        
        void dosyaYazdırma(){

            ofstream normalYazdır(name+"-normal.txt");
            ofstream bradikardiYazdır(name+"-bradikardi.txt");
            ofstream tasikardiYazdır(name+"-tasikardi.txt");

            normalYazdır << left << setw(15) << "ZAMAN" << "VOLTAJ" << endl;
            normalYazdır << string(30, '-') << endl;
            for(size_t i = 0; i < normal_T.size(); ++i) {
                normalYazdır << left << setw(15) << fixed << setprecision(4) << normal_T[i] << fixed << setprecision(4) << normal_V[i] << endl;
            }

            bradikardiYazdır << left << setw(15) << "ZAMAN" << "VOLTAJ" << endl;
            bradikardiYazdır << string(30, '-') << endl;
            for(size_t i = 0; i < bradikardi_T.size(); ++i) {
                bradikardiYazdır << left << setw(15) << fixed << setprecision(4) << bradikardi_T[i] << fixed << setprecision(4) << bradikardi_V[i] << endl;
            }

            tasikardiYazdır << left << setw(15) << "ZAMAN" << "VOLTAJ" << endl;
            tasikardiYazdır << string(30, '-') << endl;
            for(size_t i = 0; i < tasikardi_T.size(); ++i) {
                tasikardiYazdır << left << setw(15) << fixed << setprecision(4) << tasikardi_T[i] << fixed << setprecision(4) << tasikardi_V[i] << endl;
            }

            return;

        }

};


/*  The dosyaBirleştir function reads and merges three input files into a single output file. First, the input files 
(inputFile1, inputFile2, inputFile3) and the output file (outputFile) are opened. If any file cannot be opened or created,
an error message is displayed, and the process is terminated. Once the files are successfully opened, a header line and 
table formatter are added to the output file. Each input file's content is read sequentially, skipping the header lines,
and written to the output file. A visual separator (**************************) is added between the files. After all lines
are written and the files are closed, a success message is displayed. This function is designed to present all data in 
an organized manner within a merged file.*/

void dosyaBirleştir(const string& inputFile1, const string& inputFile2, const string& inputFile3, const string& outputFile) {

    ifstream inFile1(inputFile1);
    ifstream inFile2(inputFile2);
    ifstream inFile3(inputFile3);
    ofstream outFile(outputFile);

    if (!inFile1) {
        cout << "Hata: " << inputFile1 << " dosyası açılamadı!" << endl;
        return;
    }
    if (!inFile2) {
        cout << "Hata: " << inputFile2 << " dosyası açılamadı!" << endl;
        return;
    }
    if (!inFile3) {
        cout << "Hata: " << inputFile3 << " dosyası açılamadı!" << endl;
        return;
    }

    if (!outFile) {
        cout << "Hata: " << outputFile << " dosyası oluşturulamadı!" << endl;
        return;
    }

    outFile << left << setw(15) << "ZAMAN" << "VOLTAJ" << endl;
    outFile << string(30, '-') << endl;
    string line;

    int count = 0;
    while (getline(inFile1, line)) {
        if (count <= 1) {
            count ++;
            continue;
        }
        outFile << line << endl;
    }
    inFile1.close();
    outFile << "*" << endl;
    count = 0;
    while (getline(inFile2, line)) {
        if (count <= 1) {
            count ++;
            continue;
        }
        outFile << line << endl;
    }
    inFile2.close();
    outFile << "*" << endl;
    count = 0;
    while (getline(inFile3, line)) {
        if (count <= 1) {
            count ++;
            continue;
        }
        outFile << line << endl;
    }
    inFile3.close();

    cout << "Dosya islemleri tamamlandı!" << endl;
    outFile.close();
}


/*  The main function analyzes the EKG data of three different individuals and combines the classification results into a single file. First, three objects of the EKG class are created, and the analizEt function is called for each, analyzing the corresponding EKG files. This analysis process includes reading the data, detecting peak points, classifying them, and writing the results to the appropriate files.

Afterward, the files created for each class (normal, bradycardia, tachycardia) are merged using the dosyaBirleştir function, combining all individuals' data into a single file. For example, the "normal-person-1-2-3.txt" file contains the merged normal class data for all individuals. This process is repeated separately for each class.

At the end of the function, return 0; ensures that the program terminates successfully. This structure ensures a well-organized and clear execution of the analysis and merging processes. */
int main(){
    EKG person1, person2, person3;

    person1.analizEt("person1");
    person2.analizEt("person2");
    person3.analizEt("person3");

    dosyaBirleştir("person1-normal.txt", "person2-normal.txt", "person3-normal.txt", "normal-person-1-2-3.txt");
    dosyaBirleştir("person1-bradikardi.txt", "person2-bradikardi.txt", "person3-bradikardi.txt", "bradikardi-person-1-2-3.txt");
    dosyaBirleştir("person1-tasikardi.txt", "person2-tasikardi.txt", "person3-tasikardi.txt", "tasikardi-person-1-2-3.txt");


    return 0;
}
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <iomanip>

using namespace std;

#define DIR "dados_malha_aerea/"

struct Airport {
    string icao;
    string name;
    string city;
};

struct Airline {
    string icao;
    string name;
};

struct Aircraft {
    string icao;
    string model;
};

struct Flight {
    int id;
    string orig;
    string dest;
    int dep_time;
    int arr_time;
    string airline;
    string flight_number;
    string aircraft;
};

/* Aux functions */
vector<string> 
parse_csv(const string& line, char delimiter) 
{
    vector<string> result;
    bool in_quotes = false;
    string current_field = "";
    for (char c : line) {
        if (c == '"') {
            in_quotes = !in_quotes;
        } else if (c == delimiter && !in_quotes) {
            result.push_back(current_field);
            current_field.clear();
        } else {
            current_field += c;
        }
    }
    result.push_back(current_field);
    return result;
}

int 
parse_dtom(const string& datetime) 
{
    if (datetime.length() < 16) return -1;
    int day = stoi(datetime.substr(0, 2));
    int month = stoi(datetime.substr(3, 2));
    int hour = stoi(datetime.substr(11, 2));
    int minute = stoi(datetime.substr(14, 2));
    
    int days_in_month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int total_days = 0;
    
    if (month < 3) return -1; 
    
    for (int m = 3; m < month; m++) {
        total_days += days_in_month[m];
    }
    total_days += (day - 1);

    return total_days * 24 * 60 + hour * 60 + minute;
}

string 
mtos(int total_minutes) 
{
    int rem = total_minutes % (24 * 60);
    int hour = rem / 60;
    int minute = rem % 60;
    
    int total_days = total_minutes / (24 * 60);
    int days_in_month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int month = 3; // Marco zero
    int day = total_days + 1;
    
    while (day > days_in_month[month]) {
        day -= days_in_month[month];
        month++;
        if (month > 12) break;
    }
    
    char buf[30];
    snprintf(buf, sizeof(buf), "%02d/%02d/2026 %02d:%02d", day, month, hour, minute);
    return string(buf);
}

class AirNetwork {
private:
    unordered_map<string, Airport> airports;
    unordered_map<string, Airline> airlines;
    unordered_map<string, Aircraft> aircrafts;
    
    vector<Flight> all_flights;
    unordered_map<string, vector<int>> graph;
    unordered_set<string> hubs;
    
public:
    int load_data(void) {
        ifstream f_air(std::string(DIR) + "aerodromos.csv");
        if(!f_air.is_open()) return 1;
        string line;
        if (getline(f_air, line)) {
            while (getline(f_air, line)) {
                auto cols = parse_csv(line, ';');
                if (cols.size() >= 5) {
                	string country = cols[5];
                	if(country != "BRASIL") continue;
                    airports[cols[0]] = {cols[0], cols[2], cols[3]};
                }
            }
        }
        f_air.close();

        ifstream f_cia(std::string(DIR) + "cias.csv");
        if(!f_cia.is_open()) return 1;
        if (getline(f_cia, line)) {
            while (getline(f_cia, line)) {
                auto cols = parse_csv(line, ';');
                if (cols.size() >= 3) {
                	string country = cols[3];
                	if(country != "BRASIL") continue;
                    airlines[cols[0]] = {cols[0], cols[2]};
                }
            }
        }
        f_cia.close();

        ifstream f_ac(std::string(DIR) + "aeronaves.csv");
        if(!f_ac.is_open()) return 1;
        if (getline(f_ac, line)) {
            while (getline(f_ac, line)) {
                auto cols = parse_csv(line, ';');
                if (cols.size() >= 3) {
                    aircrafts[cols[0]] = {cols[0], cols[2]};
                }
            }
        }
        f_ac.close();

        ifstream f_flights(std::string(DIR) + "voos_mar2026.csv");
        if(!f_flights.is_open()) return 1;
        unordered_map<string, int> degree_count;
        int flight_id = 0;

        if (getline(f_flights, line)) {
            while (getline(f_flights, line)) {
                auto cols = parse_csv(line, ',');
                if (cols.size() >= 11) {
                	string type = cols[0];
                    string arr_str = cols[1];
                    string dep_str = cols[2];
                    string flight_num = cols[5];
                    string airline_icao = cols[7];
                    string equip_icao = cols[8];
                    string dest = cols[9];
                    string orig = cols[10];

                    if(type != "REGULAR DE PASSAGEIROS DOMÉSTICA") continue;

                    int dep_time = parse_dtom(dep_str);
                    int arr_time = parse_dtom(arr_str);

                    if (dep_time == -1 || arr_time == -1) continue;

                    all_flights.push_back({flight_id, orig, dest, dep_time, arr_time, airline_icao, flight_num, equip_icao});
                    graph[orig].push_back(flight_id);
                    
                    degree_count[orig]++;
                    degree_count[dest]++;
                    flight_id++;
                }
            }
        }
        f_flights.close();

        vector<pair<int, string>> sorted_hubs;
        for (auto const& [icao, count] : degree_count) {
            sorted_hubs.push_back({count, icao});
        }
        sort(sorted_hubs.rbegin(), sorted_hubs.rend());

        cout << "\n| Brazil's TOP 5 AIRHUBS |" << endl;
        unsigned int hubc = sorted_hubs.size();

        for (unsigned int i = 0; i < 5 && i < hubc; i++) {
            hubs.insert(sorted_hubs[i].second);
            string airport_name = airports.count(sorted_hubs[i].second) ? airports[sorted_hubs[i].second].name : "N/I";
            cout << i + 1 << ". " << sorted_hubs[i].second << " (" << airport_name << ") - " << sorted_hubs[i].first << " conexoes" << endl;
        }
        cout << "--------------------------------------------------" << endl;

        return 0;
    }

    struct State {
        int time;
        string airport;
        int flight_id;
        bool operator>(const State& other) const {
            return time > other.time;
        }
    };

    void shortest_path(const string& orig, const string& dest, const string& start_date, const string& closed_hub) {
        int start_time = parse_dtom(start_date);
        if (start_time == -1) {
            cout << "ERROR: Invalid format." << endl;
            return;
        }

        priority_queue<State, vector<State>, greater<State>> pq;
        unordered_map<string, int> earliest_arrival;
        unordered_map<string, int> parent_flight;

        pq.push({start_time, orig, -1});
        earliest_arrival[orig] = start_time;

        bool found = false;

        while (!pq.empty()) {
            auto [cur_time, u, in_flight_id] = pq.top();
            pq.pop();

            if (cur_time > earliest_arrival[u]) continue;

            if (u == dest) {
                found = true;
                break;
            }

            int min_connection = 0;
            if (u != orig) {
                min_connection = hubs.count(u) ? 60 : 45;
            }

            for (int f_id : graph[u]) {
                const Flight& f = all_flights[f_id];

                if (f.orig == closed_hub || f.dest == closed_hub) continue;

                if (f.dep_time >= cur_time + min_connection) {
                    if (earliest_arrival.find(f.dest) == earliest_arrival.end() || f.arr_time < earliest_arrival[f.dest]) {
                        earliest_arrival[f.dest] = f.arr_time;
                        parent_flight[f.dest] = f_id;
                        pq.push({f.arr_time, f.dest, f_id});
                    }
                }
            }
        }

        if (!found) {
            cout << "\nNo route for [" << orig << "] to [" << dest 
                 << "] at [" << start_date << "]." << endl;
            return;
        }

        vector<int> path_flights;
        string curr = dest;
        while (curr != orig) {
            int f_id = parent_flight[curr];
            path_flights.push_back(f_id);
            curr = all_flights[f_id].orig;
        }
        reverse(path_flights.begin(), path_flights.end());

        cout << "\n| Route Found |" << endl;
        for (size_t i = 0; i < path_flights.size(); i++) {
            const Flight& f = all_flights[path_flights[i]];
            string c_name = airlines.count(f.airline) ? airlines[f.airline].name : f.airline;
            string ac_model = aircrafts.count(f.aircraft) ? aircrafts[f.aircraft].model : f.aircraft;
            string o_name = airports.count(f.orig) ? airports[f.orig].name : f.orig;
            string d_name = airports.count(f.dest) ? airports[f.dest].name : f.dest;

            if (i > 0) {
                const Flight& prev = all_flights[path_flights[i-1]];
                int wait = f.dep_time - prev.arr_time;
                cout << "   [!] Connection in " << f.orig << " - Time in land: " << wait / 60 << "h " << wait % 60 << "m" << endl;
            }
            
            cout << "[" << c_name << " | Flight " << f.flight_number << " | " << ac_model << "]" << endl;
            cout << " -> Departure: " << o_name << " (" << f.orig << ") as " << mtos(f.dep_time) << endl;
            cout << " -> Arrival: " << d_name << " (" << f.dest << ") as " << mtos(f.arr_time) << endl;
        }
        
        int total_duration = all_flights[path_flights.back()].arr_time - all_flights[path_flights.front()].dep_time;
        cout << "--------------------------------------------------" << endl;
        cout << "\nTotal travel time: " << total_duration / 60 << "h " << total_duration % 60 << "m" << "\n" << endl;
        cout << "--------------------------------------------------" << endl;
    }
};

int 
main(void) 
{
    AirNetwork net;
    int err = net.load_data();
    if(err) {
    	cout << "ERROR: Failed to load .csv files. Maybe they're missing?" << endl;
    	return 0;
    }

    cout << "A list of Brazil's airports ICAO codes can be found at the following link: \n" 
    	 << "https://airportsbase.org/Brazil/all/airports/page5" << endl;

    while (true) {
        string orig, dest, date, time_str, closed_hub;
        char close_choice;

		cout << "--------------------------------------------------" << endl;
        cout << "Specify origin ICAO (ex: SBPA) or 'LEAVE': ";
        cin >> orig;
        if (orig == "LEAVE") break;
        
        cout << "Specify destination ICAO (ex: SBEG): ";
        cin >> dest;
        
        cout << "Desired Date (DD/MM/YYYY): ";
        cin >> date;
        
        cout << "Desired time (HH:MM): ";
        cin >> time_str;

		cout << "Do you want to remove one of the 5 HUBS from your search? (Y/N) ";
        cin >> close_choice;
        if (toupper(close_choice) == 'Y') {
            cout << "Specify the hub's ICAO: ";
            cin >> closed_hub;
        }

        string full_date = date + " " + time_str;
        net.shortest_path(orig, dest, full_date, closed_hub);
    }
    
    return 0;
}

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <cmath>
#include <climits>
#include <algorithm>

using namespace std;

const string TEHRAN = "Tehran";
const string STATUS_WAREHOUSE = "WAREHOUSE";
const string STATUS_INTRANSIT = "INTRANSIT";
const string STATUS_DELIVERED = "DELIVERED";
const int SCORE_WEIGHT_FACTOR = 5;
const double EPS_COMPARE = 1e-9;

struct Truck
{
    int id;
    int capacity;
    int remaining_capacity;
    string current_city;
    vector<int> orders;
    bool in_transit;
};

struct Order
{
    int id;
    int weight;
    int score;
    string origin_city;
    string current_city;
    string destination_city;
    string status;
};

struct City
{
    string city_name;
    int distance;
};

vector<string> split_lines(const string &line, char delimiter)
{
    vector<string> cells;
    stringstream readline(line);
    string cell;
    while (getline(readline, cell, delimiter))
    {
        cells.push_back(cell);
    }
    return cells;
}

vector<vector<string>> read_CSV(const string &file_name)
{
    vector<vector<string>> data;
    string line;
    char delimiter = ',';
    ifstream file(file_name);

    if (!file.is_open())
    {
        cerr << "Error!" << endl;
        return data;
    }

    while (getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }
        data.push_back(split_lines(line, delimiter));
    }

    return data;
}

vector<Truck> read_trucks(const string &file_name)
{
    vector<vector<string>> raw = read_CSV(file_name);
    vector<Truck> trucks;
    for (size_t i = 1; i < raw.size(); i++)
    {
        if (raw[i].size() < 2)
        {
            continue;
        }
        Truck truck;
        truck.id = stoi(raw[i][0]);
        truck.capacity = stoi(raw[i][1]);
        truck.remaining_capacity = truck.capacity;
        truck.current_city = TEHRAN;
        truck.in_transit = false;
        trucks.push_back(truck);
    }
    return trucks;
}

map<string, int> read_cities(const string &file_name)
{
    vector<vector<string>> raw = read_CSV(file_name);
    map<string, int> cities;
    for (size_t i = 1; i < raw.size(); i++)
    {
        if (raw[i].size() < 2)
        {
            continue;
        }
        City new_city;
        new_city.city_name = raw[i][0];
        new_city.distance = stoi(raw[i][1]);
        cities[new_city.city_name] = new_city.distance;
    }

    return cities;
}

void add_order(vector<Order> &orders, string origin_city, string destination_city, int weight, int &largest_order_id)
{

    Order new_order;
    largest_order_id++;
    new_order.id = largest_order_id;
    new_order.origin_city = origin_city;
    new_order.destination_city = destination_city;
    new_order.weight = weight;
    new_order.status = STATUS_WAREHOUSE;
    new_order.current_city = origin_city;

    orders.push_back(new_order);
}

void order_tracking(int order_id, int &largest_order_id, vector<Order> &orders)
{
    if (order_id < 1 || order_id > largest_order_id)
    {
        cout << "Order not found";
        return;
    }
    int index = order_id - 1;
    if (orders[index].status == STATUS_WAREHOUSE)
    {
        cout << "Order " << order_id << " is currently in warehouse in " << orders[index].origin_city << endl;
    }
    else if (orders[index].status == STATUS_INTRANSIT)
    {
        cout << "Order " << order_id << " is in transit to " << orders[index].current_city << endl;
    }
    else if (orders[index].status == STATUS_DELIVERED)
    {
        cout << "Order " << order_id << " is delivered to " << orders[index].destination_city << endl;
    }
}

vector<Order> eligible_orders(const vector<Order> &orders, const string &current_city, const string &destination_city)
{
    vector<Order> eligible;
    for (size_t i = 0; i < orders.size(); i++)
    {
        if (orders[i].status == STATUS_WAREHOUSE && orders[i].destination_city == destination_city && orders[i].current_city == current_city)
        {
            eligible.push_back(orders[i]);
        }
    }
    return eligible;
}

int score_calculation(int largest_order_id, int order_id, int weight)
{
    return weight + (largest_order_id - order_id) * SCORE_WEIGHT_FACTOR;
}

void calculate_scores(vector<Order> &eligible_orders, int largest_order_id)
{
    for (size_t i = 0; i < eligible_orders.size(); i++)
    {
        eligible_orders[i].score = eligible_orders[i].weight + (largest_order_id - eligible_orders[i].id) * SCORE_WEIGHT_FACTOR;
    }
}

void sort_orders(vector<Order> &eligible_orders)
{
    sort(eligible_orders.begin(), eligible_orders.end(),
         [](const Order &a, const Order &b)
         {
             if (a.score != b.score)
                 return a.score > b.score;
             return a.id < b.id;
         });
}

vector<int> load_orders(Truck &truck, const vector<Order> &eligible_orders, vector<Order> &orders, const string &destination_city)
{
    vector<int> loaded_ids;
    for (size_t i = 0; i < eligible_orders.size(); i++)
    {
        if (truck.remaining_capacity < eligible_orders[i].weight)
        {
            continue;
        }
        truck.remaining_capacity -= eligible_orders[i].weight;
        truck.orders.push_back(eligible_orders[i].id);
        loaded_ids.push_back(eligible_orders[i].id);
        orders[eligible_orders[i].id - 1].status = STATUS_INTRANSIT;
        orders[eligible_orders[i].id - 1].current_city = destination_city;
    }
    if (!loaded_ids.empty())
    {
        truck.in_transit = true;
        truck.current_city = destination_city;
    }
    return loaded_ids;
}

void print_loading_result(int truck_id, const vector<int> &loaded_ids)
{
    if (loaded_ids.empty())
    {
        cout << "No order could be loaded" << endl;
        return;
    }
    vector<int> sorted_ids = loaded_ids;
    sort(sorted_ids.begin(), sorted_ids.end());
    cout << "Truck " << truck_id << " loaded with orders:";
    for (size_t i = 0; i < sorted_ids.size(); i++)
    {
        cout << " " << sorted_ids[i];
    }
    cout << endl;
}

void truck_loading(vector<Order> &orders, vector<Truck> &trucks, int truck_id, string destination_city, int largest_order_id)
{

    int index = truck_id - 1;
    if (trucks[index].in_transit)
    {
        cout << "No order could be loaded" << endl;
        return;
    }
    vector<Order> eligible_vector = eligible_orders(orders, trucks[index].current_city, destination_city);
    if (eligible_vector.empty())
    {
        cout << "No order could be loaded" << endl;
        return;
    }
    calculate_scores(eligible_vector, largest_order_id);
    sort_orders(eligible_vector);
    vector<int> loaded_ids = load_orders(trucks[index], eligible_vector, orders, destination_city);
    print_loading_result(truck_id, loaded_ids);
}

void delivery(vector<Order> &orders, vector<Truck> &trucks, int truck_id)
{
    int index = truck_id - 1;
    if (trucks[index].orders.empty())
    {
        cout << "No orders to deliver in truck " << truck_id << endl;
        return;
    }
    sort(trucks[index].orders.begin(), trucks[index].orders.end());
    cout << "Truck " << truck_id << " delivered orders: ";
    for (size_t i = 0; i < trucks[index].orders.size(); i++)
    {
        cout << trucks[index].orders[i] << " ";
        orders[trucks[index].orders[i] - 1].status = STATUS_DELIVERED;
        trucks[index].remaining_capacity += orders[trucks[index].orders[i] - 1].weight;
    }
    cout << endl;
    trucks[index].in_transit = false;
    trucks[index].orders.clear();
}

int sum_scores_between(const vector<Order> &orders, const string &origin, const string &dest, int largest_order_id)
{
    int total = 0;
    for (size_t i = 0; i < orders.size(); i++)
    {
        if (orders[i].status == STATUS_WAREHOUSE && orders[i].origin_city == origin && orders[i].destination_city == dest)
        {
            total += score_calculation(largest_order_id, orders[i].id, orders[i].weight);
        }
    }
    return total;
}

int total_weight_to_dest(const vector<Order> &orders, const string &dest)
{
    int total = 0;
    for (size_t i = 0; i < orders.size(); i++)
    {
        if (orders[i].status == STATUS_WAREHOUSE && orders[i].origin_city == TEHRAN && orders[i].destination_city == dest)
        {
            total += orders[i].weight;
        }
    }
    return total;
}

string find_best_city(const vector<Order> &orders, const map<string, int> &cities, int largest_order_id)
{
    string best_city = "";
    double best_score = -1.0;

    for (const auto &entry : cities)
    {
        string city = entry.first;
        int dist = entry.second;
        if (city == TEHRAN || dist == 0)
            continue;

        int in_score = sum_scores_between(orders, TEHRAN, city, largest_order_id);
        int out_score = sum_scores_between(orders, city, TEHRAN, largest_order_id);
        double city_score = (in_score + out_score) / (double)dist;

        if (city_score > best_score + EPS_COMPARE)
        {
            best_score = city_score;
            best_city = city;
        }
        else if (fabs(city_score - best_score) < EPS_COMPARE && (best_city.empty() || city < best_city))
        {
            best_city = city;
        }
    }
    return best_city;
}

int find_best_truck(const vector<Truck> &trucks, int required_weight)
{
    int best_truck_id = -1;
    int min_remaining = INT_MAX;

    for (size_t i = 0; i < trucks.size(); i++)
    {
        if (trucks[i].current_city == TEHRAN && !trucks[i].in_transit && trucks[i].remaining_capacity >= required_weight)
        {
            int rem = trucks[i].remaining_capacity;
            if (rem < min_remaining || (rem == min_remaining && trucks[i].id < best_truck_id))
            {
                min_remaining = rem;
                best_truck_id = trucks[i].id;
            }
        }
    }
    return best_truck_id;
}

void recommend(const vector<Order> &orders, const vector<Truck> &trucks, const map<string, int> &cities, int largest_order_id)
{
    string best_city = find_best_city(orders, cities, largest_order_id);
    int required_weight = total_weight_to_dest(orders, best_city);
    int best_truck = find_best_truck(trucks, required_weight);

    cout << "Recommended city: " << best_city << endl;
    cout << "Recommended truck: " << best_truck << endl;
}

int calculate_order_cost(const Order &order, const map<string, int> &cities)
{
    return cities.at(order.destination_city) * order.weight;
}

void financial_report(const vector<Order> &orders, const map<string, int> &cities)
{
    long long total_income = 0;
    vector<pair<int, int>> delivered_orders;

    for (size_t i = 0; i < orders.size(); i++)
    {
        if (orders[i].status == STATUS_DELIVERED)
        {
            int cost = calculate_order_cost(orders[i], cities);
            total_income += cost;
            delivered_orders.push_back({orders[i].id, cost});
        }
    }

    sort(delivered_orders.begin(), delivered_orders.end(),
         [](const pair<int, int> &a, const pair<int, int> &b)
         {
             return a.first < b.first;
         });

    cout << "Total income: " << total_income << endl;
    cout << "Delivered orders:" << endl;
    for (size_t i = 0; i < delivered_orders.size(); i++)
    {
        cout << delivered_orders[i].first << " " << delivered_orders[i].second << endl;
    }
}

void get_inputs(string command, int &largest_order_id, vector<Order> &orders, vector<Truck> &trucks, const map<string, int> &cities)
{
    if (command == "add_order")
    {
        string origin_city, destination_city;
        int weight;
        cin >> origin_city >> destination_city >> weight;
        add_order(orders, origin_city, destination_city, weight, largest_order_id);
        cout << "Order " << largest_order_id << " added" << endl;
    }
    else if (command == "track")
    {
        int order_id;
        cin >> order_id;
        order_tracking(order_id, largest_order_id, orders);
    }
    else if (command == "load")
    {
        int truck_id;
        string destination_city;
        cin >> truck_id >> destination_city;
        truck_loading(orders, trucks, truck_id, destination_city, largest_order_id);
    }
    else if (command == "deliver")
    {
        int truck_id;
        cin >> truck_id;
        delivery(orders, trucks, truck_id);
    }
    else if (command == "recommend")
    {
        recommend(orders, trucks, cities, largest_order_id);
    }
    else if (command == "financial_report")
    {
        financial_report(orders, cities);
    }
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        return 1;
    }
    vector<Truck> trucks = read_trucks(argv[1]);
    map<string, int> cities = read_cities(argv[2]);
    vector<Order> orders;
    string command;
    int largest_order_id = 0;
    while (cin >> command)
    {
        get_inputs(command, largest_order_id, orders, trucks, cities);
    }

    return 0;
}

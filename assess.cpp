#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <string>
#include <sstream>
#include "json.hpp" // Include the JSON library

using json = nlohmann::json;
using namespace std;

struct Point {
    int x;
    long long y;
};

// Function to decode a number from a specific base to base 10
long long decodeValue(const string& value, int base) {
    long long result = 0;
    for (char c : value) {
        int digit = (c >= '0' && c <= '9') ? (c - '0') : (c - 'a' + 10);
        result = result * base + digit;
    }
    return result;
}

// Function to perform Lagrange Interpolation at x = 0 to find constant term
long long lagrangeInterpolation(const vector<Point>& points) {
    double result = 0.0;

    for (size_t i = 0; i < points.size(); ++i) {
        int xi = points[i].x;
        long long yi = points[i].y;

        // Calculate Lagrange basis polynomial L_i(0)
        double li = 1.0;
        for (size_t j = 0; j < points.size(); ++j) {
            if (i != j) {
                int xj = points[j].x;
                li *= (0 - xj) / static_cast<double>(xi - xj);
            }
        }
        result += yi * li;
    }

    return static_cast<long long>(result + 0.5); // Round to nearest integer
}

int main() {
    // Load JSON data
    ifstream inputFile("input.json");
    json inputData;
    inputFile >> inputData;

    // Read n and k values
    int n = inputData["keys"]["n"];
    int k = inputData["keys"]["k"];

    // Parse points and decode the y-values
    vector<Point> points;
    for (int i = 1; i <= n; ++i) {
        string baseStr = inputData[to_string(i)]["base"];
        string valueStr = inputData[to_string(i)]["value"];

        int x = i;
        int base = stoi(baseStr);
        long long y = decodeValue(valueStr, base);
        
        points.push_back({x, y});
    }

    // Use the first k points for Lagrange interpolation to find the constant term
    vector<Point> selectedPoints(points.begin(), points.begin() + k);
    long long secret = lagrangeInterpolation(selectedPoints);

    cout << "The constant term (secret) is: " << secret << endl;

    return 0;
}


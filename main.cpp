#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <string>

using namespace std;

// FCFS algorithm
int fcfs(const vector<int>& requests, int head, vector<int>& sequence) {
    int total_movement = 0;
    int current = head;

    for (int r : requests) {
        total_movement += abs(current - r);
        sequence.push_back(r);
        current = r;
    }

    return total_movement;
}

// SCAN algorithm
int scan(vector<int> requests, int head, int max_cylinder, vector<int>& sequence, char direction) {
    int total_movement = 0;
    int current = head;

    sort(requests.begin(), requests.end());
    vector<int> left, right;

    for (int r : requests) {
        if (r < head)
            left.push_back(r);
        else
            right.push_back(r);
    }

    if (direction == 'r' || direction == 'R') {
        // Move right first
        for (int r : right) {
            total_movement += abs(current - r);
            sequence.push_back(r);
            current = r;
        }

        if (current != max_cylinder) {
            total_movement += abs(current - max_cylinder);
            current = max_cylinder;
            sequence.push_back(current);
        }

        reverse(left.begin(), left.end());
        for (int r : left) {
            total_movement += abs(current - r);
            sequence.push_back(r);
            current = r;
        }
    } else {
        // Move left first
        reverse(left.begin(), left.end());
        for (int r : left) {
            total_movement += abs(current - r);
            sequence.push_back(r);
            current = r;
        }

        if (current != 0) {
            total_movement += current;
            current = 0;
            sequence.push_back(0);
        }

        for (int r : right) {
            total_movement += abs(current - r);
            sequence.push_back(r);
            current = r;
        }
    }

    return total_movement;
}

// C-SCAN algorithm
int c_scan(vector<int> requests, int head, int max_cylinder, vector<int>& sequence, char direction) {
    int total_movement = 0;
    int current = head;

    sort(requests.begin(), requests.end());
    vector<int> left, right;

    for (int r : requests) {
        if (r < head)
            left.push_back(r);
        else
            right.push_back(r);
    }

    if (direction == 'r' || direction == 'R') {
        // Move right first, then jump to beginning
        for (int r : right) {
            total_movement += abs(current - r);
            sequence.push_back(r);
            current = r;
        }

        if (current != max_cylinder) {
            total_movement += abs(current - max_cylinder);
            sequence.push_back(max_cylinder);
        }

        total_movement += max_cylinder; // Jump to start
        current = 0;
        sequence.push_back(0);

        for (int r : left) {
            total_movement += abs(current - r);
            sequence.push_back(r);
            current = r;
        }
    } else {
        // Move left first, then jump to end
        reverse(left.begin(), left.end());
        for (int r : left) {
            total_movement += abs(current - r);
            sequence.push_back(r);
            current = r;
        }

        if (current != 0) {
            total_movement += current;
            sequence.push_back(0);
        }

        total_movement += max_cylinder; // Jump to end
        current = max_cylinder;
        sequence.push_back(max_cylinder);

        reverse(right.begin(), right.end());
        for (int r : right) {
            total_movement += abs(current - r);
            sequence.push_back(r);
            current = r;
        }
    }

    return total_movement;
}

int main() {
    int head, num_requests, max_cylinder;
    string algorithm;
    char direction = 'r'; // Default direction is right

    cout << "Enter the number of cylinders (e.g., 5000): ";
    cin >> max_cylinder;

    if (max_cylinder <= 0) {
        cerr << "Error: Number of cylinders must be positive.\n";
        return 1;
    }

    cout << "Enter initial head position (0:" << max_cylinder - 1 << "): ";
    cin >> head;

    if (head < 0 || head >= max_cylinder) {
        cerr << "Error: Head position must be between 0 and " << max_cylinder - 1 << ".\n";
        return 1;
    }

    cout << "Enter number of disk requests: ";
    cin >> num_requests;

    if (num_requests <= 0) {
        cerr << "Error: Number of requests must be positive.\n";
        return 1;
    }

    vector<int> requests(num_requests);
    cout << "Enter the disk requests (each between 0 and " << max_cylinder - 1 << "):\n";
    for (int i = 0; i < num_requests; ++i) {
        cin >> requests[i];
        if (requests[i] < 0 || requests[i] >= max_cylinder) {
            cerr << "Error: Invalid request at index " << i << ". Must be between 0 and " << max_cylinder - 1 << ".\n";
            return 1;
        }
    }

    cout << "\nChoose algorithm (FCFS, SCAN, C-SCAN): ";
    cin >> algorithm;

    for (auto& ch : algorithm) ch = toupper(ch);

    if (algorithm == "SCAN" || algorithm == "C-SCAN" || algorithm == "CSCAN") {
        cout << "Choose initial direction (r for right/higher cylinders, l for left/lower cylinders): ";
        cin >> direction;

        if (direction != 'r' && direction != 'R' && direction != 'l' && direction != 'L') {
            cerr << "Error: Invalid direction. Using default direction (right).\n";
            direction = 'r';
        }
    }

    vector<int> sequence;
    int total_movement = 0;

    cout << "\nInitial Head Position: " << head << "\n";

    if (algorithm == "FCFS") {
        total_movement = fcfs(requests, head, sequence);
        cout << "Algorithm: FCFS\n";
    } else if (algorithm == "SCAN") {
        total_movement = scan(requests, head, max_cylinder - 1, sequence, direction);
        cout << "Algorithm: SCAN\n";
        cout << "Initial Direction: " << (direction == 'r' || direction == 'R' ? "Right (higher)" : "Left (lower)") << "\n";
    } else if (algorithm == "C-SCAN" || algorithm == "CSCAN") {
        total_movement = c_scan(requests, head, max_cylinder - 1, sequence, direction);
        cout << "Algorithm: C-SCAN\n";
        cout << "Initial Direction: " << (direction == 'r' || direction == 'R' ? "Right (higher)" : "Left (lower)") << "\n";
    } else {
        cerr << "Error: Invalid algorithm name. Use FCFS, SCAN, or C-SCAN.\n";
        return 1;
    }

    cout << "Total Head Movement: " << total_movement << " cylinders\n";
    cout << "Sequence of Served Requests: " << head;
    for (int r : sequence) {
        cout << " -> " << r;
    }
    cout << "\n";

    return 0;
}

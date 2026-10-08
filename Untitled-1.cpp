#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <cmath>
#include <string>

using namespace std;

// ======================================================
// DRONE CLASS
// ======================================================

class Drone
{
public:
    int id;
    string name;

    double battery;
    double energyPerKm;

    bool available;

    double x;
    double y;

    Drone(int i, string n, double b, double e,
          double px, double py)
    {
        id = i;
        name = n;
        battery = b;
        energyPerKm = e;

        available = true;

        x = px;
        y = py;
    }
};


// ======================================================
// DELIVERY REQUEST CLASS
// ======================================================

class DeliveryRequest
{
public:
    int requestId;

    string customerName;

    double sourceX;
    double sourceY;

    double destinationX;
    double destinationY;

    int priority;
    int waitingTime;

    DeliveryRequest(int id,
                    string customer,
                    double sx,
                    double sy,
                    double dx,
                    double dy,
                    int p,
                    int wait)
    {
        requestId = id;
        customerName = customer;

        sourceX = sx;
        sourceY = sy;

        destinationX = dx;
        destinationY = dy;

        priority = p;
        waitingTime = wait;
    }
};


// ======================================================
// PRIORITY QUEUE COMPARATOR
// ======================================================

struct RequestComparator
{
    bool operator()(DeliveryRequest a,
                    DeliveryRequest b)
    {
        // Higher priority comes first

        if (a.priority != b.priority)
        {
            return a.priority < b.priority;
        }

        // If priority is same,
        // higher waiting time comes first

        return a.waitingTime < b.waitingTime;
    }
};


// ======================================================
// DRONE DELIVERY SYSTEM
// ======================================================

class DroneDeliverySystem
{
private:

    // Priority Queue
    priority_queue<
        DeliveryRequest,
        vector<DeliveryRequest>,
        RequestComparator
    > requestQueue;


    // Drone database
    unordered_map<int, Drone> drones;


    // Hub position
    double hubX;
    double hubY;


public:

    // ==================================================
    // CONSTRUCTOR
    // ==================================================

    DroneDeliverySystem()
    {
        hubX = 0;
        hubY = 0;
    }


    // ==================================================
    // ADD DRONE
    // ==================================================

    void addDrone(Drone d)
    {
        drones[d.id] = d;
    }


    // ==================================================
    // ADD DELIVERY REQUEST
    // ==================================================

    void addRequest(DeliveryRequest request)
    {
        requestQueue.push(request);

        cout << "\nRequest "
             << request.requestId
             << " added successfully.";
    }


    // ==================================================
    // DISTANCE CALCULATION
    // ==================================================

    double calculateDistance(double x1,
                             double y1,
                             double x2,
                             double y2)
    {
        double dx = x2 - x1;
        double dy = y2 - y1;

        return sqrt(dx * dx + dy * dy);
    }


    // ==================================================
    // BATTERY FEASIBILITY CHECK
    // ==================================================

    bool batteryCheck(Drone d,
                      DeliveryRequest request)
    {
        // Drone → Pickup
        double distanceToSource =
            calculateDistance(
                d.x,
                d.y,
                request.sourceX,
                request.sourceY
            );


        // Pickup → Customer
        double deliveryDistance =
            calculateDistance(
                request.sourceX,
                request.sourceY,
                request.destinationX,
                request.destinationY
            );


        // Customer → Hub
        double returnDistance =
            calculateDistance(
                request.destinationX,
                request.destinationY,
                hubX,
                hubY
            );


        // Total mission distance
        double totalDistance =
            distanceToSource +
            deliveryDistance +
            returnDistance;


        // Energy required
        double requiredEnergy =
            totalDistance * d.energyPerKm;


        // 20% safety reserve
        double safetyReserve =
            requiredEnergy * 0.20;


        // Total required battery
        double totalRequired =
            requiredEnergy + safetyReserve;


        cout << "\n\nDrone ID: "
             << d.id;

        cout << "\nTotal Distance: "
             << totalDistance
             << " km";

        cout << "\nRequired Energy: "
             << requiredEnergy
             << "%";

        cout << "\nSafety Reserve: "
             << safetyReserve
             << "%";

        cout << "\nTotal Battery Required: "
             << totalRequired
             << "%";

        cout << "\nAvailable Battery: "
             << d.battery
             << "%";


        // Battery Gate

        if (d.battery >= totalRequired)
        {
            cout << "\nBattery Check: PASS";

            return true;
        }
        else
        {
            cout << "\nBattery Check: FAIL";

            return false;
        }
    }


    // ==================================================
    // SUITABILITY SCORE
    // ==================================================

    double calculateSuitabilityScore(
        Drone d,
        DeliveryRequest request)
    {
        // Distance from drone to pickup
        double distance =
            calculateDistance(
                d.x,
                d.y,
                request.sourceX,
                request.sourceY
            );


        // Delivery distance
        double deliveryDistance =
            calculateDistance(
                request.sourceX,
                request.sourceY,
                request.destinationX,
                request.destinationY
            );


        // Energy required
        double energy =
            (distance + deliveryDistance)
            * d.energyPerKm;


        /*
            Suitability Score

            Lower score = better drone

            Distance       → 2 points
            Energy         → 1.5 points
            Waiting Time   → reduces score
        */

        double score =
            distance * 2
            + energy * 1.5
            - request.waitingTime * 0.5;


        return score;
    }


    // ==================================================
    // BEST DRONE SELECTION
    // ==================================================

    void selectBestDrone()
    {
        if (requestQueue.empty())
        {
            cout << "\nNo delivery request available.";

            return;
        }


        // Get highest priority request

        DeliveryRequest request =
            requestQueue.top();

        requestQueue.pop();


        cout << "\n\n====================================";
        cout << "\n      PROCESSING DELIVERY REQUEST";
        cout << "\n====================================";


        cout << "\nRequest ID: "
             << request.requestId;

        cout << "\nCustomer: "
             << request.customerName;

        cout << "\nPriority: "
             << request.priority;

        cout << "\nWaiting Time: "
             << request.waitingTime
             << " minutes";


        int bestDroneId = -1;

        double bestScore = 999999;


        cout << "\n\n====================================";
        cout << "\n       DRONE EVALUATION";
        cout << "\n====================================";


        // Check every drone

        for (auto &item : drones)
        {
            Drone d = item.second;


            // Ignore unavailable drones

            if (!d.available)
            {
                continue;
            }


            // ------------------------------------------
            // STEP 1: BATTERY CHECK
            // ------------------------------------------

            bool batteryPass =
                batteryCheck(
                    d,
                    request
                );


            // If battery fails,
            // drone cannot be selected

            if (!batteryPass)
            {
                cout << "\nDrone "
                     << d.id
                     << " rejected.";

                continue;
            }


            // ------------------------------------------
            // STEP 2: SUITABILITY SCORE
            // ------------------------------------------

            double score =
                calculateSuitabilityScore(
                    d,
                    request
                );


            cout << "\nSuitability Score: "
                 << score;


            // ------------------------------------------
            // STEP 3: FIND LOWEST SCORE
            // ------------------------------------------

            if (score < bestScore)
            {
                bestScore = score;

                bestDroneId = d.id;
            }
        }


        // =================================================
        // FINAL RESULT
        // =================================================

        cout << "\n\n====================================";


        if (bestDroneId == -1)
        {
            cout << "\nNO SUITABLE DRONE AVAILABLE";
        }
        else
        {
            cout << "\n        BEST DRONE SELECTED";

            cout << "\n====================================";

            cout << "\nDrone ID: "
                 << bestDroneId;

            cout << "\nDrone Name: "
                 << drones[bestDroneId].name;

            cout << "\nSuitability Score: "
                 << bestScore;
        }


        cout << "\n====================================\n";
    }
};


// ======================================================
// MAIN FUNCTION
// ======================================================

int main()
{
    DroneDeliverySystem system;


    // ==================================================
    // ADD DRONES
    // ==================================================

    Drone d1(
        101,
        "Drone-A",
        100,       // Battery
        3,         // Energy per km
        0,         // X
        0          // Y
    );


    Drone d2(
        102,
        "Drone-B",
        80,
        4,
        2,
        1
    );


    Drone d3(
        103,
        "Drone-C",
        60,
        2.5,
        1,
        3
    );


    system.addDrone(d1);
    system.addDrone(d2);
    system.addDrone(d3);


    // ==================================================
    // ADD DELIVERY REQUESTS
    // ==================================================

    DeliveryRequest r1(
        1,
        "Customer A",

        2, 2,       // Pickup

        8, 7,       // Destination

        3,          // Priority

        10          // Waiting time
    );


    DeliveryRequest r2(
        2,
        "Customer B",

        1, 1,

        5, 6,

        1,

        20
    );


    DeliveryRequest r3(
        3,
        "Customer C",

        3, 2,

        7, 8,

        2,

        5
    );


    system.addRequest(r1);
    system.addRequest(r2);
    system.addRequest(r3);


    // ==================================================
    // SELECT BEST DRONE
    // ==================================================

    system.selectBestDrone();


    return 0;
}
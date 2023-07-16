#include <algorithm>
#include <iostream>
#include <vector>

// Place - содержит значимость места и то, сколько времени надо покуколдить в нём
struct Place {
    int64_t significance;
    int64_t timeToVisit;
};

// Route - хранит сколько надо пиздохать от одного места до другого
struct Route {
    size_t beginID;
    size_t endID;
    int64_t duration;
};

// Маршрут движения, общая значимость и занимаемое время экскурсии
struct Excursion {
    std::vector<size_t> placesIDs;
    int64_t significance{0};
    int64_t duration{0};
};

class GraphEngine {
public:
    GraphEngine(const std::vector<Place> &_places, const std::vector<Route> &_routes) :
            places(_places) {
        for (const auto &[beginID, endID, duration]: _routes) {
            routes.push_back({beginID, endID, duration});
            routes.push_back({endID, beginID, duration});
        }
    }

    auto getAllSatisfyingPaths(size_t startPlace, int64_t maxTime = 60) {
        if (adjList.empty())
            buildAdjList();
        std::vector<Excursion> allExcursions;
        dfsWithoutRemembering(startPlace, {}, allExcursions, maxTime);
        std::sort(allExcursions.rbegin(), allExcursions.rend(),
                  [](const auto &lhs, const auto &rhs) {
                      return lhs.significance == rhs.significance ? lhs.duration > rhs.duration : lhs.significance <
                                                                                                  rhs.significance;
                  });
        return allExcursions;
    }

    ~GraphEngine() = default;

private:
    void buildAdjList() {
        adjList.resize(places.size());
        for (const auto &route: routes)
            adjList[route.beginID].push_back(route);
    }

    void
    dfsWithoutRemembering(size_t placeID, Excursion excursion, std::vector<Excursion> &allExcursions,
                          int64_t maxTime, int64_t routeDuration = 0) {
        excursion.duration += routeDuration + places[placeID].timeToVisit;
        if (std::find(excursion.placesIDs.begin(), excursion.placesIDs.end(), placeID) != excursion.placesIDs.end())
            return;
        if (excursion.duration > maxTime)
            return;
        excursion.placesIDs.push_back(placeID);
        excursion.significance += places[placeID].significance;
        allExcursions.push_back(excursion);
        for (const auto &[beginID, endID, duration]: adjList[placeID])
            dfsWithoutRemembering(endID, excursion, allExcursions, maxTime, duration);
    }

    std::vector<Place> places;
    std::vector<Route> routes;
    std::vector<std::vector<Route>> adjList;
};

int main() {
    GraphEngine graphEngine(
            {
                    {5, 2},
                    {3, 2},
                    {6, 2},
                    {6, 3},
                    {3, 2},
                    {6, 4},
                    {6, 4},
                    {4, 2},
            },
            {
                    {0, 1, 2},
                    {0, 2, 2},
                    {1, 2, 1},
                    {2, 3, 2},
                    {3, 4, 2},
                    {3, 5, 3},
                    {4, 5, 2},
                    {5, 6, 5},
                    {6, 7, 2},
                    {0, 3, 4},
                    {2, 5, 5},
                    {3, 6, 8},
                    {5, 7, 5}
            }
    );
    auto allExcursions = graphEngine.getAllSatisfyingPaths(6, 40);
    int i = 0;
    for (const auto &[placesIDs, significance, duration]: allExcursions) {
        std::cout << "S:" << significance << " D:" << duration << " P: ";
        bool first = true;
        for (size_t placeID: placesIDs) {
            if (first)
                first = false;
            else
                std::cout << "->";
            std::cout << placeID;
        }
        std::cout << std::endl;
        if (++i > 5)
            break;
    }
    return 0;
}

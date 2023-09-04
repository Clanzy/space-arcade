#include "grid.hpp"

#include "settings.hpp"

#include <SDL.h>

#include <algorithm>
#include <iterator>
#include <map>
#include <unordered_set>
#include <vector>

namespace Grid
{

namespace
{
	
constexpr int grid_rows = 10;
constexpr int grid_columns = 10;

int cell_width = settings::initial_screen_width / grid_rows;
int cell_height = settings::initial_screen_height / grid_columns;

struct Point {
	int row;
	int column;

	Point(const float r, const float c)
		: row{ static_cast<int>(r) }
		, column{ static_cast<int>(c) }
	{
	}
	Point &operator=(const Point &) = default;
	bool operator<(const Point &other) const
	{
		return this->row < other.row ||
		       (this->row == other.row && this->column < other.column);
	}
};

std::vector<std::vector<std::set<int> > > grid(grid_rows + 5,
					       std::vector<std::set<int> >(grid_columns + 5));

std::multimap<int, Point> reverse_grid;

inline SDL_Point fpoint_to_point(const SDL_FPoint &fpoint)
{
	return SDL_Point{ static_cast<int>(fpoint.x), static_cast<int>(fpoint.y) };
}

} // namespace

void move_element(Entity::entity_id element, const Components::Position &p, const Components::Collision &c)
{
	const SDL_FRect frect = { p.x + 100.F, p.y + 100.F, c.w, c.h };

	std::set<Point> points;
	points.insert({ frect.y / cell_height, frect.x / cell_width });
	points.insert({ frect.y / cell_height, (frect.x + frect.w) / cell_width });
	points.insert({ (frect.y + frect.h) / cell_height, frect.x / cell_width });
	points.insert({ (frect.y + frect.h) / cell_height, (frect.x + frect.w) / cell_width });

	std::set<Point> previous_points;
	auto range = reverse_grid.equal_range(element);
	for (auto it = range.first; it != range.second; ++it) {
		previous_points.insert(it->second);
	}

	std::set<Point> delete_from;
	std::set_difference(previous_points.begin(), previous_points.end(), points.begin(),
			    points.end(), std::inserter(delete_from, delete_from.begin()));

	std::set<Point> insert_into;
	std::set_difference(points.begin(), points.end(), previous_points.begin(),
			    previous_points.end(), std::inserter(insert_into, insert_into.begin()));

	for (auto p : delete_from) { grid[p.row][p.column].erase(element); }
	for (auto p : insert_into) { grid[p.row][p.column].insert(element); }

	reverse_grid.erase(range.first, range.second);
	for (auto p : points) { reverse_grid.insert({ element, p }); };
}

void remove_element(Entity::entity_id element)
{
	auto range = reverse_grid.equal_range(element);
	for (auto it = range.first; it != range.second; ++it) {
		grid[it->second.row][it->second.column].erase(element);
	}
	reverse_grid.erase(element);
}

std::set<Entity::entity_id> get_neighbour_objects(const Entity::entity_id element)
{
	std::set<Entity::entity_id> ret;
	auto range = reverse_grid.equal_range(element);
	for (auto it = range.first; it != range.second; ++it) {
		for (auto i : grid[it->second.row][it->second.column]) { ret.insert(i); }
	}
	ret.erase(element);
	return ret;
}

} // namespace Grid

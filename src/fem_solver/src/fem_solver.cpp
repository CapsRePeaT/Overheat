#include "fem_solver.hpp"

#include <chrono>

#include "variance_solver.hpp"
#include "heatmap_converter.hpp"
#include "matrix_agregator.hpp"
#include "main_matrix_solver.hpp"

void FemSolver::Solve(FileRepresentation& file_rep, SolverSetup setup) {
	if (setup.calculate_test_geometry) {
		double dummy = 10;
		auto dummy_f = [](const double step) -> double {
			return step;
		};
		GeometryCutter cutter(dummy, dummy_f, dummy_f);
		auto geom_db = cutter.PrepareTestGeometry();
		const auto index_2_coord_map = cutter.GetVerticeIndexes();
		MatrixEquation main_matrix(index_2_coord_map.MaxIndex() + 1);
		main_matrix.set_known_temp_and_indexes(cutter.TestTempAndIndexes());
		for (const auto& element : geom_db.elements())
			element->AddElementContribution(main_matrix);
		const auto heatmap = main_matrix.Solve(setup.matrix_solver_type);
		heatmap.Print();
		geom_db.SetHeatmap(heatmap);
		geom_db.SetVerticeIndexes(cutter.GetVerticeIndexes());
		file_rep.set_fs_datapack(std::move(geom_db));
	} else {
		// cutting
		auto timer_start = std::chrono::high_resolution_clock::now();
		std::cout << "starting heat solving..." << std::endl;
		std::cout << "Geometry cutting and element contribution started." << std::endl;
		auto corner_points_step = setup.corner_points_step;
		corner_points_step *= 1000;
		const double area_step = std::pow(corner_points_step, 2) / 2;
		const double volume_by_formula = (1.0 / 12) * std::pow(area_step, 3) * std::sqrt(2.0);
		const double volume_step = volume_by_formula +volume_by_formula / 10;
		auto area_constraint = [&area_step](const double step) -> double {
			return area_step;
		};
		// regular tetrahedron volume
		auto volume_constraint = [&volume_step](const double step) -> double {
			return volume_step;
		};
		GeometryCutter cutter(corner_points_step, area_constraint, volume_constraint);
		auto geom_db = cutter.PrepareGeometry(file_rep, setup.show_triangulation /* show debug view*/);
		auto timer_cutter_fin = std::chrono::high_resolution_clock::now();
		std::cout << "Geometry cutting and element contribution computation fineshed, it took " 
						<< std::chrono::duration_cast<std::chrono::seconds>(timer_cutter_fin - timer_start)
						<< " seconds." << std::endl;
		geom_db.PrintContent();
		const auto index_2_coord_map = cutter.GetVerticeIndexes();
		MatrixEquation main_matrix(index_2_coord_map.MaxIndex() + 1);
		main_matrix.set_known_temp_and_indexes(cutter.TempAndIndexes());
		for (const auto& element : geom_db.elements())
			element->AddElementContribution(main_matrix);
		auto timer_matrix_filling_fin = std::chrono::high_resolution_clock::now();
		std::cout << "Adding element contribution to main matrix fineshed, it took "
			<< std::chrono::duration_cast<std::chrono::seconds>(timer_matrix_filling_fin - timer_cutter_fin)
			<< " seconds." << std::endl;
		const auto heatmap = main_matrix.Solve(setup.matrix_solver_type);
		auto timer_matrix_fin = std::chrono::high_resolution_clock::now();
		std::cout << "Main matrix soliving fineshed, it took "
			<< std::chrono::duration_cast<std::chrono::seconds>(timer_matrix_fin - timer_matrix_filling_fin)
			<< " seconds." << std::endl;
		heatmap.Print();
		geom_db.SetHeatmap(heatmap);
		geom_db.SetVerticeIndexes(cutter.GetVerticeIndexes());
		auto timer_fin = std::chrono::high_resolution_clock::now();
		geom_db.set_solver_runtime_sec(std::chrono::duration_cast<std::chrono::milliseconds>(timer_fin - timer_start));
		file_rep.set_fs_datapack(std::move(geom_db));
		std::cout << "Whole loading and computation took "
			<< geom_db.solver_runtime_sec().count() / 1000.0
			<< " seconds." << std::endl;
		file_rep.set_fs_datapack(std::move(geom_db));
	}
};

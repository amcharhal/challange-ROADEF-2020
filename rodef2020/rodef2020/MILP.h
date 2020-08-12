#pragma once

#include <iostream>
#include <ilcplex/ilocplex.h>
#include "Instance.h"
#include "Data.h"
#include "Solution.h"

class MILP
{
	friend std::ostream& operator<<(std::ostream& ar_stream, const MILP& ar_milp);
public:
	MILP(Instance* ap_instance, bool a_obj1 = true);
	~MILP();

	///// SOLVER /////
	void createModelMILP();
	Solution solveMILP();

private:
	//--- Instance ---//
	Instance* mp_instance;
	Data* mp_data;
	bool m_obj1;

	//--- getters ---//
	Data* getData() { return mp_data; }
	double getY_it(int a_i, int a_t) const { return m_cplex.getValue(m_y_it[a_i][a_t - 1]); }
	double getX_it(int a_i, int a_t) const { return m_cplex.getValue(m_x_it[a_i][a_t - 1]); }

	//--- Solver environement ---//
	IloEnv m_env;
	IloModel m_model;
	IloCplex m_cplex;

	//--- MILP Variables ---//
	IloArray<IloNumVarArray> m_y_it;
	IloArray<IloNumVarArray> m_x_it;
	IloArray<IloArray<IloNumVarArray>> m_w_ict;
	IloArray<IloArray<IloNumVarArray>> m_r_ict;
	IloArray<IloArray<IloNumVarArray>> m_d_its;
	IloArray<IloArray<IloNumVarArray>> m_p_its;

	///// SOLVER /////
	void createVariables();
	void createObjectiveFunction();
	void createConstraints();
};


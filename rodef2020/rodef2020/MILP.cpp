#include <algorithm>
#include <ilcplex/ilocplex.h>
ILOSTLBEGIN
#include "MILP.h"
#include "Exclusion.h"
#include "Tools.h"


MILP::MILP(Instance* ap_instance, bool a_obj1):
	mp_instance(ap_instance),
	mp_data(ap_instance->getData()),
	m_obj1(a_obj1)
{
	//// solver environement initialization
	m_model = IloModel(m_env);
	m_cplex = IloCplex(m_model);
}


MILP::~MILP()
{
	m_env.end();
}

/* Create the MILP model on the instance */
void MILP::createModelMILP()
{
	// create the model
	createVariables();
	createObjectiveFunction();
	createConstraints();

	// save the model
	string path = OUTPUT_FOLDER + mp_instance->getFolder() + mp_instance->getName() + MODEL_MILP + LP_EXTENSION;
	m_cplex.exportModel(path.c_str());
}

/* Solve the MILP, save the cplex solution file and create a Solution object corresponding to the cplex solution */
Solution MILP::solveMILP()
{
	// solve the MILP
	m_cplex.setParam(IloCplex::Param::TimeLimit, MAX_CPLEX_TIME);
	m_cplex.solve();

	// save the MILP solution
	string path = OUTPUT_FOLDER + mp_instance->getFolder() + mp_instance->getName() + SOLUTION_MILP + TXT_EXTENSION;
	m_cplex.writeSolution(path.c_str());

	// create and save a Solution object
	if (IloAlgorithm::Status::Optimal == m_cplex.getStatus() ||
		IloAlgorithm::Status::Feasible == m_cplex.getStatus())
	{
		Solution sol(mp_data);

		for (int i = 0; i < mp_data->getNbInterventions(); i++)
		{
			for (int t = 1; t <= mp_data->getT(); t++)
			{
				if (-TOLERANCE <= m_cplex.getValue(m_y_it[i][t - 1]) && m_cplex.getValue(m_y_it[i][t - 1]) <= TOLERANCE)
				{
					// not the real starting time !
				}
				else
				{
					// intervention i start at time t
					sol.setStartingTime(i, t);
				}
			}
			for (int t = 1; t <= mp_data->getT(); t++)
			{
				if (-TOLERANCE <= m_cplex.getValue(m_x_it[i][t - 1]) && m_cplex.getValue(m_x_it[i][t - 1]) <= TOLERANCE)
				{
					sol.setXit(i, t, false);
				}
				else
				{
					// intervention i is in process at time t
					sol.setXit(i, t, true);
				}
			}
		}

		return sol;
	}
	else
	{
		stringstream message;
		message << "The MILP has no feasible solution ! " << m_cplex.getStatus() << endl;
		throw exception(message.str().c_str());
	}
}

void MILP::createVariables()
{
	///// y_it for all i in I, t in H /////
	m_y_it = IloArray<IloNumVarArray>(m_env, mp_data->getNbInterventions());
	for (int i = 0; i < mp_data->getNbInterventions(); i++)
	{
		m_y_it[i] = IloNumVarArray(m_env, mp_data->getT());
		for (int t = 1; t <= mp_data->getT(); t++)
		{
			stringstream name;
			name << "y(" << i << "," << t << ")";
			m_y_it[i][t - 1] = IloNumVar(m_env, 0.0, 1.0, ILOBOOL, name.str().c_str());
			m_model.add(m_y_it[i][t - 1]);
		}
	}

	///// x_it for all i in I, t in H /////
	m_x_it = IloArray<IloNumVarArray>(m_env, mp_data->getNbInterventions());
	for (int i = 0; i < mp_data->getNbInterventions(); i++)
	{
		m_x_it[i] = IloNumVarArray(m_env, mp_data->getT());
		for (int t = 1; t <= mp_data->getT(); t++)
		{
			stringstream name;
			name << "x(" << i << "," << t << ")";
			m_x_it[i][t - 1] = IloNumVar(m_env, 0.0, 1.0, ILOBOOL, name.str().c_str());
			m_model.add(m_x_it[i][t - 1]);
		}
	}

	///// w_ict for all i in I, c in C, t in H /////
	m_w_ict = IloArray<IloArray<IloNumVarArray>>(m_env, mp_data->getNbInterventions());
	for (int i = 0; i < mp_data->getNbInterventions(); i++)
	{
		m_w_ict[i] = IloArray<IloNumVarArray>(m_env, mp_data->getNbResources());
		for (int c = 0; c < mp_data->getNbResources(); c++)
		{
			m_w_ict[i][c] = IloNumVarArray(m_env, mp_data->getT());
			for (int t = 1; t <= mp_data->getT(); t++)
			{
				stringstream name;
				name << "w(" << i << "," << c << "," << t << ")";
				m_w_ict[i][c][t - 1] = IloNumVar(m_env, 0.0, IloInfinity, ILOFLOAT, name.str().c_str());
				m_model.add(m_w_ict[i][c][t - 1]);
			}
		}
	}

	///// r_ict for all i in I, c in C, t in H /////
	m_r_ict = IloArray<IloArray<IloNumVarArray>>(m_env, mp_data->getNbInterventions());
	for (int i = 0; i < mp_data->getNbInterventions(); i++)
	{
		m_r_ict[i] = IloArray<IloNumVarArray>(m_env, mp_data->getNbResources());
		for (int c = 0; c < mp_data->getNbResources(); c++)
		{
			m_r_ict[i][c] = IloNumVarArray(m_env, mp_data->getT());
			for (int t = 1; t <= mp_data->getT(); t++)
			{
				stringstream name;
				name << "r(" << i << "," << c << "," << t << ")";
				m_r_ict[i][c][t - 1] = IloNumVar(m_env, 0.0, IloInfinity, ILOFLOAT, name.str().c_str());
				m_model.add(m_r_ict[i][c][t - 1]);
			}
		}
	}

	if (true == m_obj1)
	{
		///// d_its for all i in I, t in H, s in St /////
		m_d_its = IloArray<IloArray<IloNumVarArray>>(m_env, mp_data->getNbInterventions());
		for (int i = 0; i < mp_data->getNbInterventions(); i++)
		{
			m_d_its[i] = IloArray<IloNumVarArray>(m_env, mp_data->getT());
			for (int t = 1; t <= mp_data->getT(); t++)
			{
				m_d_its[i][t - 1] = IloNumVarArray(m_env, mp_data->getScenariosNumber(t));
				for (int s = 0; s < mp_data->getScenariosNumber(t); s++)
				{
					stringstream name;
					name << "d(" << i << "," << t << "," << s << ")";
					m_d_its[i][t - 1][s] = IloNumVar(m_env, 0.0, IloInfinity, ILOFLOAT, name.str().c_str());
					m_model.add(m_d_its[i][t - 1][s]);
				}
			}
		}

		///// p_its for all i in I, t in H, s in St /////
		m_p_its = IloArray<IloArray<IloNumVarArray>>(m_env, mp_data->getNbInterventions());
		for (int i = 0; i < mp_data->getNbInterventions(); i++)
		{
			m_p_its[i] = IloArray<IloNumVarArray>(m_env, mp_data->getT());
			for (int t = 1; t <= mp_data->getT(); t++)
			{
				m_p_its[i][t - 1] = IloNumVarArray(m_env, mp_data->getScenariosNumber(t));
				for (int s = 0; s < mp_data->getScenariosNumber(t); s++)
				{
					stringstream name;
					name << "p(" << i << "," << t << "," << s << ")";
					m_p_its[i][t - 1][s] = IloNumVar(m_env, 0.0, IloInfinity, ILOFLOAT, name.str().c_str());
					m_model.add(m_p_its[i][t - 1][s]);
				}
			}
		}
	}
}

void MILP::createObjectiveFunction()
{
	if (true == m_obj1)
	{
		// objective 1
		IloExpr exprObj1(m_env);
		for (int t = 1; t <= mp_data->getT(); t++)
		{
			IloExpr exprS(m_env);
			for (int s = 0; s < mp_data->getScenariosNumber(t); s++)
			{
				IloExpr exprI(m_env);
				for (int i = 0; i < mp_data->getNbInterventions(); i++)
				{
					exprI += m_p_its[i][t - 1][s];
				}
				exprS += exprI;
			}
			exprS /= mp_data->getScenariosNumber(t);
			exprObj1 += exprS;
		}
		exprObj1 /= mp_data->getT();

		IloObjective obj(m_env, exprObj1, IloObjective::Minimize, "OBJ");
		m_model.add(obj);
	}
}

void MILP::createConstraints()
{
	///// 1.1 Non-preemptive scheduling (1)-(2) /////
	///// (1) -> if i starts at t, then i is processed at periods {t,...t + delta_it - 1}
	for (int i = 0; i < mp_data->getNbInterventions(); i++)
	{
		for (int t = 1; t <= mp_data->getT(); t++)
		{
			IloExpr expr1(m_env);
			expr1 = m_y_it[i][t - 1] * mp_data->getIntervention(i)->getDelta(t);

			IloExpr expr2(m_env);
			int maxT2 = min(mp_data->getT(), t + mp_data->getIntervention(i)->getDelta(t) - 1);
			for (int t2 = t; t2 <= maxT2; t2++)
			{
				expr2 += m_x_it[i][t2 - 1];
			}

			stringstream name;
			name << "C_non_preemptive_1(" << i << "," << t << ")";
			IloRange non_preemptive_1(m_env, 0, expr2 - expr1, IloInfinity, name.str().c_str());
			m_model.add(non_preemptive_1);
		}
	}

	///// (2) -> if i starts at t, then i can not be processed at a period out of {t,...t + delta_it - 1}
	for (int i = 0; i < mp_data->getNbInterventions(); i++)
	{
		for (int t = 1; t <= mp_data->getT(); t++)
		{
			IloExpr expr1(m_env);
			for (int t2 = 1; t2 <= t-1; t2++)
			{
				expr1 += m_x_it[i][t2 - 1];
			}

			IloExpr expr2(m_env);
			for (int t2 = t + mp_data->getIntervention(i)->getDelta(t); t2 <= mp_data->getT(); t2++)
			{
				expr2 += m_x_it[i][t2 - 1];
			}

			IloExpr expr3(m_env);
			int maxi = max(mp_data->getIntervention(i)->getDelta());
			expr3 = maxi * (1 - m_y_it[i][t - 1]);

			stringstream name;
			name << "C_non_preemptive_2(" << i << "," << t << ")";
			IloRange non_preemptive_2(m_env, 0, expr3 - expr2 - expr1, IloInfinity, name.str().c_str());
			m_model.add(non_preemptive_2);
		}
	}

	///// 1.2 Interventions are scheduled once (3) /////
	for (int i = 0; i < mp_data->getNbInterventions(); i++)
	{
		IloExpr expr(m_env);
		for (int t = 1; t <= mp_data->getT(); t++)
		{
			expr += m_y_it[i][t - 1];
		}
		stringstream name;
		name << "C_scheduled_once(" << i << ")";
		IloRange scheduled_once(m_env, 1, expr, 1, name.str().c_str());
		m_model.add(scheduled_once);
	}

	///// 1.3 No work left (4) /////
	for (int i = 0; i < mp_data->getNbInterventions(); i++)
	{
		IloExpr expr(m_env);
		for (int t = 1; t <= mp_data->getT(); t++)
		{
			expr += m_y_it[i][t - 1] * t;
		}
		stringstream name;
		name << "C_no_work_left(" << i << ")";
		IloRange no_work_left(m_env, 0, expr, mp_data->getIntervention(i)->getTMax(), name.str().c_str());
		m_model.add(no_work_left);
	}


	///// 2. Resource constraints (5)-(9) /////
	///// (5) -> the total allocated workload to resource c on period t is bounded: l_t^c <= r_t^c <= u_t^c
	for (int c = 0; c < mp_data->getNbResources(); c++)
	{
		for (int t = 1; t <= mp_data->getT(); t++)
		{
			IloExpr expr(m_env);
			for (int i = 0; i < mp_data->getNbInterventions(); i++)
			{
				expr += m_r_ict[i][c][t - 1];
			}

			// lower bound
			stringstream nameLB;
			nameLB << "C_resource_constraints_5_LB(" << c << "," << t << ")";
			IloRange resource_constraints_5_LB(m_env, mp_data->getResource(c)->getMin(t), expr, IloInfinity, nameLB.str().c_str());
			m_model.add(resource_constraints_5_LB);
			// upper bound
			stringstream nameUB;
			nameUB << "C_resource_constraints_5_UB(" << c << "," << t << ")";
			IloRange resource_constraints_5_UB(m_env, 0, expr, mp_data->getResource(c)->getMax(t), nameUB.str().c_str());
			m_model.add(resource_constraints_5_UB);
		}
	}

	///// (6) -> the required workload allocation to an intervention i for resource c on period t
	for (int i = 0; i < mp_data->getNbInterventions(); i++)
	{
		for (int c = 0; c < mp_data->getNbResources(); c++)
		{
			for (int t = 1; t <= mp_data->getT(); t++)
			{
				IloExpr expr(m_env);
				for (int t2 = 1; t2 <= mp_data->getT(); t2++)
				{
					expr += m_y_it[i][t2 - 1] * mp_data->getIntervention(i)->getWorkload(c, t, t2);
				}
				expr -= m_w_ict[i][c][t - 1];

				stringstream name;
				name << "C_resource_constraints_6(" << i << "," << c << "," << t << ")";
				IloRange resource_constraints_6(m_env, 0, expr, 0, name.str().c_str());
				m_model.add(resource_constraints_6);
			}
		}
	}

	/* 
	 * Constraints from (7) to (9) are the linearization constraints of r_i^ct = x_it * w_i^ct which counts the
	 * workload allocated to intervention i for resource c on period t if and only if intervention i is in process
	 * at period t
	*/
	///// (7) -> r_i^ct <= M * x_it
	for (int i = 0; i < mp_data->getNbInterventions(); i++)
	{
		for (int c = 0; c < mp_data->getNbResources(); c++)
		{
			for (int t = 1; t <= mp_data->getT(); t++)
			{
				IloExpr expr(m_env);
				expr = BIG_M * m_x_it[i][t - 1] - m_r_ict[i][c][t - 1];

				stringstream name;
				name << "C_resource_constraints_7(" << i << "," << c << "," << t << ")";
				IloRange resource_constraints_7(m_env, 0, expr, IloInfinity, name.str().c_str());
				m_model.add(resource_constraints_7);
			}
		}
	}

	///// (8) -> r_i^ct <= w_i^ct
	for (int i = 0; i < mp_data->getNbInterventions(); i++)
	{
		for (int c = 0; c < mp_data->getNbResources(); c++)
		{
			for (int t = 1; t <= mp_data->getT(); t++)
			{
				IloExpr expr(m_env);
				expr = m_w_ict[i][c][t - 1] - m_r_ict[i][c][t - 1];

				stringstream name;
				name << "C_resource_constraints_8(" << i << "," << c << "," << t << ")";
				IloRange resource_constraints_8(m_env, 0, expr, IloInfinity, name.str().c_str());
				m_model.add(resource_constraints_8);
			}
		}
	}

	///// (9) -> r_i^ct >= w_i^ct - M * (1 - x_it)
	for (int i = 0; i < mp_data->getNbInterventions(); i++)
	{
		for (int c = 0; c < mp_data->getNbResources(); c++)
		{
			for (int t = 1; t <= mp_data->getT(); t++)
			{
				IloExpr expr(m_env);
				expr = m_r_ict[i][c][t - 1] - m_w_ict[i][c][t - 1] + BIG_M * (1 - m_x_it[i][t - 1]);

				stringstream name;
				name << "C_resource_constraints_9(" << i << "," << c << "," << t << ")";
				IloRange resource_constraints_9(m_env, 0, expr, IloInfinity, name.str().c_str());
				m_model.add(resource_constraints_9);
			}
		}
	}


	///// 3. Disjonctive constraints (10) /////
	for (int i = 0; i < mp_data->getNbExclusions(); i++)
	{
		Exclusion* p_exc = mp_data->getExclusion(i);
		int i1 = p_exc->getI1();
		int i2 = p_exc->getI2();
		E_Season season = p_exc->getSeason();
		vector<int>* p_concernedPeriods = 0;	// pointer on the vector containing the periods of exclusion

		switch (season)
		{
		case (WINTER):
			p_concernedPeriods = mp_data->getSeasons()->getWinter();	break;
		case (SUMMER):
			p_concernedPeriods = mp_data->getSeasons()->getSummer();	break;
		case (IS):
			p_concernedPeriods = mp_data->getSeasons()->getIs();	break;
		case (FULL):
			p_concernedPeriods = mp_data->getSeasons()->getFull();	break;
		}

		for (int t : *p_concernedPeriods)
		{
			IloExpr expr(m_env);
			expr += m_x_it[i1][t - 1] + m_x_it[i2][t - 1];

			stringstream name;
			name << "C_exclusion(" << i1 << "," << i2 << "," << t << ")";
			IloRange exclusion(m_env, 0, expr, 1, name.str().c_str());
			m_model.add(exclusion);
		}
	}


	///// 4. Risk computation constraints (11)-(14) /////
	///// (11) -> the total risk allocation to an intervention i for scenario s on period t
	for (int i = 0; i < mp_data->getNbInterventions(); i++)
	{
		for (int t = 1; t <= mp_data->getT(); t++)
		{
			for (int s = 0; s < mp_data->getScenariosNumber(t); s++)
			{
				IloExpr expr(m_env);
				for (int t2 = 1; t2 <= mp_data->getT(); t2++)
				{
					expr += m_y_it[i][t2 - 1] * mp_data->getIntervention(i)->getRisk(t, t2, s);
				}
				expr -= m_d_its[i][t - 1][s];

				stringstream name;
				name << "C_risk_constraints_11(" << i << "," << t << "," << s << ")";
				IloRange risk_constraints_11(m_env, 0, expr, 0, name.str().c_str());
				m_model.add(risk_constraints_11);
			}
		}
	}

	/*
	 * Constraints from (12) to (14) are the linearization constraints of p_i^ts = x_it * d_i^ts which counts the
	 * risk allocated to intervention i for scenario s on period t if and only if intervention i is in process at
	 * period t
	*/
	///// (12) -> p_i^ts <= M * x_it
	for (int i = 0; i < mp_data->getNbInterventions(); i++)
	{
		for (int t = 1; t <= mp_data->getT(); t++)
		{
			for (int s = 0; s < mp_data->getScenariosNumber(t); s++)
			{
				IloExpr expr(m_env);
				expr = BIG_M * m_x_it[i][t - 1] - m_p_its[i][t - 1][s];

				stringstream name;
				name << "C_risk_constraints_12(" << i << "," << t << "," << s << ")";
				IloRange risk_constraints_12(m_env, 0, expr, IloInfinity, name.str().c_str());
				m_model.add(risk_constraints_12);
			}
		}
	}

	///// (13) -> p_i^ts <= d_i^ts
	for (int i = 0; i < mp_data->getNbInterventions(); i++)
	{
		for (int t = 1; t <= mp_data->getT(); t++)
		{
			for (int s = 0; s < mp_data->getScenariosNumber(t); s++)
			{
				IloExpr expr(m_env);
				expr = m_d_its[i][t - 1][s] - m_p_its[i][t - 1][s];

				stringstream name;
				name << "C_risk_constraints_13(" << i << "," << t << "," << s << ")";
				IloRange risk_constraints_13(m_env, 0, expr, IloInfinity, name.str().c_str());
				m_model.add(risk_constraints_13);
			}
		}
	}

	///// (14) -> p_i^ts >= d_i^ts - M * (1 - x_it)
	for (int i = 0; i < mp_data->getNbInterventions(); i++)
	{
		for (int t = 1; t <= mp_data->getT(); t++)
		{
			for (int s = 0; s < mp_data->getScenariosNumber(t); s++)
			{
				IloExpr expr(m_env);
				expr = m_p_its[i][t - 1][s] - m_d_its[i][t - 1][s] + BIG_M * (1 - m_x_it[i][t - 1]);

				stringstream name;
				name << "C_risk_constraints_14(" << i << "," << t << "," << s << ")";
				IloRange risk_constraints_14(m_env, 0, expr, IloInfinity, name.str().c_str());
				m_model.add(risk_constraints_14);
			}
		}
	}
}

std::ostream& operator<<(std::ostream& ar_stream, const MILP& ar_milp)
{
	if ((IloAlgorithm::Status::Optimal == ar_milp.m_cplex.getStatus()) ||
		(IloAlgorithm::Status::Feasible == ar_milp.m_cplex.getStatus()))
	{
		ar_stream << endl << "The MILP is feasible: " << ar_milp.m_cplex.getStatus() << endl;

		for (int i = 0; i < ar_milp.mp_data->getNbInterventions(); i++)
		{
			for (int t = 1; t <= ar_milp.mp_data->getT(); t++)
			{
				if (0 != ar_milp.m_cplex.getValue(ar_milp.m_y_it[i][t - 1]))
				{
					ar_stream << ar_milp.mp_data->getIntervention(i)->getName() <<
						" (" << i << ") starts at " << t << " and is in process during periods: ";
				}
			}
			for (int t = 1; t <= ar_milp.mp_data->getT(); t++)
			{
				if (0 != ar_milp.m_cplex.getValue(ar_milp.m_x_it[i][t - 1]))
				{
					ar_stream << t << SPACE;
				}
			}
			ar_stream << endl;
		}
		ar_stream << "Objective value = " << ar_milp.m_cplex.getObjValue() << endl;
	}
	else
	{
		stringstream message;
		message << "The MILP has no feasible solution ! " << ar_milp.m_cplex.getStatus();
		throw exception(message.str().c_str());
	}
}
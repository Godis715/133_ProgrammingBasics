#include "Presenter.h"

#include <fstream>

namespace DrawProject {

	Array<ID> CreatePoint(const Array<ID>& obj, const Array<double>& params) {
		if (params.GetSize() != 2) {
			return Array<ID>(0);
		}
		ID id = Model::GetInstance()->CreatePrimitive(ot_point, params);
		return CreateArr(id);
	}
	Array<ID> CreateSegment(const Array<ID>& obj, const Array<double>& params) {
		if (params.GetSize() != 4) {
			return Array<ID>(0);
		}
		ID id = Model::GetInstance()->CreatePrimitive(ot_segment, params);
		auto res = CreateArr(id);
		res += PrimController::GetInstance()->GetChildren(id);
		return res;
	}
	Array<ID> CreateArc(const Array<ID>& obj, const Array<double>& params) {
		if (params.GetSize() != 6) {
			return Array<ID>(0);
		}
		Vector2 center(params[0], params[1]);
		Vector2 start(params[2], params[3]);
		Vector2 end(params[4], params[5]);
		double angle = Vector2::Angle(start - center, end - center);
		Array<double> paramsForCreating(5);
		paramsForCreating[0] = params[0];
		paramsForCreating[1] = params[1];
		paramsForCreating[2] = params[2];
		paramsForCreating[3] = params[3];
		paramsForCreating[4] = angle;
		ID id = Model::GetInstance()->CreatePrimitive(ot_arc, paramsForCreating);
		auto res = CreateArr(id);
		res += PrimController::GetInstance()->GetChildren(id);
		return res;
	}
	Array<ID> CreateCircle(const Array<ID>& obj, const Array<double>& params) {
		if (params.GetSize() != 3) {
			return Array<ID>(0);
		}
		ID id = Model::GetInstance()->CreatePrimitive(ot_circle, params);
		auto res = CreateArr(id);
		res += PrimController::GetInstance()->GetChildren(id);
		return res;
	}
	Array<ID> Move(const Array<ID>& obj, const Array<double>& params) {
		if (params.GetSize() != 2) {
			throw std::exception("invalid arguments");
		}
		Model::GetInstance()->Move(obj, Vector2(params[0], params[1]));
		return Array<ID>(0);

	}
	Array<ID> Scale(const Array<ID>& obj, const Array<double>& params) {
		if (params.GetSize() != 1) {
			throw std::exception("invalid arguments");
		}
		Model::GetInstance()->Scale(obj, params[0]);
		Model::GetInstance()->Scale(obj, params[0]);
		return Array<ID>(0);
	}
	Array<ID> DistBetPoints(const Array<ID>& obj, const Array<double>& params) {
		ID id = Model::GetInstance()->CreateRequirement(ot_distBetPoints, obj, params);
		return CreateArr(id);
	}
	Array<ID> EqualSegment(const Array<ID>& obj, const Array<double>& params) {
		ID id = Model::GetInstance()->CreateRequirement(ot_equalSegmentLen, obj, params);
		return CreateArr(id);
	}
	Array<ID> DistanceBetPointSegment(const Array<ID>& obj, const Array<double>& params) {
		ID id = Model::GetInstance()->CreateRequirement(ot_distBetPointSeg, obj, params);
		return CreateArr(id);
	}
	Array<ID> Delete(const Array<ID>& obj, const Array<double>& params) {
		for (int i = 0; i < obj.GetSize(); ++i) {
			DataController::GetInstance()->DeleteObject(obj[i]);
		}
		return Array<ID>(0);
	}


	Presenter::Presenter() {

	}

	IView* Presenter::GetView()
	{
		return view;
	}

	Presenter* Presenter::GetInstance() {
		if (instance == nullptr) {
			instance = new Presenter();
		}
		return instance;
	}

	void Presenter::Initializer(IView* _view)
	{
		view = _view;
		
		mode = new Selection();

		model = Model::GetInstance();
		
		auto tree = new treeFunc;
		tree->Add("Point", CreatePoint);
		tree->Add("Segment", CreateSegment);
		tree->Add("Arc", CreateArc);
		tree->Add("Circle", CreateCircle);
		tree->Add("Move", Move);
		tree->Add("Scale", Scale);
		tree->Add("Dist_bet_points", DistBetPoints);
		tree->Add("Equal_segment", EqualSegment);
		tree->Add("Distance_bet_point_segment", DistanceBetPointSegment);
		tree->Add("Delete", Delete);
		Compiler::Initializer(tree);

	}

	void Presenter::CleareScene() {
		Undo_Redo::GetInstance()->Clear();
		model->Clear();
	}

	void Presenter::DrawSelectedObjects(const Array<ID>& selectedObjects)
	{
		for (int i = 0; i < selectedObjects.GetSize(); i++)
		{
			ID obj = selectedObjects[i];
			object_type type = model->GetObjType(selectedObjects[i]);
			switch (type)
			{
			case ot_point: {
				auto params = model->GetVariableObjParam(obj, VERTEX);
				view->DrawPoint(params);
				break;
			}
			case ot_segment: {
				auto params = model->GetVariableObjParam(obj, VERTEX);
				view->DrawLine(params);
				break;
			}
			case ot_arc: {
				auto params = model->GetVariableObjParam(obj, VERTEX, CENTER, RADIUS, ANGLE);
				view->DrawArc(params);
				break;
			}
			case ot_circle: {
				auto params = model->GetVariableObjParam(obj, CENTER, RADIUS);
				view->DrawCircle(params);
				break;
			}
			case ot_curve: {
				auto params = model->GetVariableObjParam(obj, VERTEX);
				view->DrawCurve(params);
				break;
			}
			default:
				break;
			}
		}
	}


	void Presenter::DrawScene()
	{
		/*std::ofstream log("Log.txt", std::ios_base::app);
		log << "DrawScene" << std::endl;
		log.close();*/
		auto color = Color::GetInstance();
		auto objCtrl = ObjectController::GetInstance();
		auto iter = model->GetPrimIterator();
		if (iter.IsValid()) {
		
			view->SetStyleDrawing(color->Primitives(), solid);
		}
		while (iter.IsValid()) {
			ID obj = *iter;
			++iter;
			if (!objCtrl->IsValid(obj)) {
				continue;
			}
			if (model->GetObjType(obj) == ot_point) {
				auto params = model->GetVariableObjParam(obj, VERTEX);
				view->DrawPoint(params);
			}
			if (model->GetObjType(obj) == ot_segment) {
				auto params = model->GetVariableObjParam(obj, VERTEX);
				view->DrawLine(params);
			}
			if (model->GetObjType(obj) == ot_arc) {
				auto params = model->GetVariableObjParam(obj, VERTEX, CENTER, RADIUS, ANGLE);
				view->DrawArc(params);
			}
			if (model->GetObjType(obj) == ot_circle) {
				auto params = model->GetVariableObjParam(obj, CENTER, RADIUS);
				view->DrawCircle(params);
			}
			if (model->GetObjType(obj) == ot_curve) {
				auto params = model->GetVariableObjParam(obj, VERTEX);
				view->DrawCurve(params);
			}
		}

		mode->DrawMode();
	}

	void Presenter::Set_event(Event _ev, Array<double>& _params, const std::string& str)
	{
	/*	std::ofstream log("Log.txt", std::ios_base::app);
		log.close();*/
		/*std::ofstream log("Log.txt", std::ios_base::app);
		log << "Set_event" << std::endl;
		log.close();*/
		Mode* temp = mode->HandleEvent(_ev, _params);
	/*	log.open("Log.txt", std::ios_base::app);
		log << ((int)temp) << std::endl;
		log.close();*/
		if (temp != nullptr)
		{
			delete mode;
			mode = temp;
		}
		
		switch (_ev)
		{
		case ev_newFile:
		{
			CleareScene();
			model->NewFile(str);
			break;
		}
		case ev_addFile:
		{
			model->AddFile(str);
			break;
		}
		case ev_openFile:
		{
			CleareScene();
			model->OpenFile(str);
			break;
		}
		case ev_saveAsFile:
		{
			model->SaveAs(str);
			break;
		}
		case ev_saveFile:
		{
			model->Save();
			break;
		}
		case ev_compile:
		{
			Presenter::Compile(str);
			break;
		}
		case ev_delAll:
		{
			CleareScene();
			break;
		}
		case ev_set_theme:
		{
			Color::GetInstance()->SetTheme(int(_params[0]));
			break;
		}
		}
		view->Update();
	}

	void Presenter::Compile(const std::string& path) {
		Compiler* compiler = Compiler::GetInstance();
		std::ifstream file;
		file.open(path);
		if (file.is_open()) {
			if (!file.eof()) {
				compiler->Parse(file);
			}
			file.close();
		}
	}

	Presenter* Presenter::instance = nullptr;
}

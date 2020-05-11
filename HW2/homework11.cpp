#include "head.h"
enum BoardStatus
{
	BLACK,
	WHITE,
	EMPTY,
	UNKNOWN_STATUS
};

enum PieceColor
{
	BLACK_P,
	WHITE_P,
	UNKNOWN_COLOR
};

enum GameMode
{
	SINGLE,
	GAME,
	UNKNOWN_MODE
};

enum ActionType 
{
    MOVE,
	JUMP,
	UNKNOWN_ACTION
};

struct Coordinate
{
	int r = 0;
	int c = 0;
	Coordinate() = default;
	Coordinate(int row, int col) {
		r = row;
		c = col;
	}
	int GetRow() {
		return r;
	}
	int GetCol() {
		return c;
	}
};

struct ActionCoordinate
{
	int r1 = 0;
	int c1 = 0;
	int r2 = 0;
	int c2 = 0;
	ActionCoordinate(int a, int b, int c, int d) {
		r1 = a;
		c1 = b;
		r2 = c;
		c2 = d;
	}
};

vector<int> direction_vector_row = { -1,0,1,1,1,0,-1,-1 };
vector<int> direction_vector_column = { -1,-1,-1,0,1,1,1,0 };
bool flag = false;//仅在有棋子在营内且无法出营或远离营地时变为true

double DoubleMax(double a, double b) {
	if (a > b)
		return a;
	else
	{
		return b;
	}
}

double DoubleMin(double a, double b) {
	if (a > b)
		return b;
	else
	{
		return a;
	}
}

class Piece {
public:
	Piece() = default;
    Piece(int r, int c, PieceColor p_color, int p_status) : piece_coordinate(r, c), color(p_color), status(p_status) {}
	int GetStatus() {
		return this->status;
	}
	PieceColor GetColor() {
		return this->color;
	}
	Coordinate GetCoordinate() {
		return this->piece_coordinate;
	}
	void SetStatus(int a) {
		this->status = a;
	}
	void SetColor(PieceColor a) {
		this->color = a;
	}
	void SetCoordinate(int r, int c) {
		this->piece_coordinate.r = r;
		this->piece_coordinate.c = c;
	}
	void ShowPiece(ostream& cout) {
		cout << "color:" << this->color << " row:" << this->piece_coordinate.r << " column:" << this->piece_coordinate.c;
		cout << " status:" << this->status << endl;
	}
private:
	Coordinate piece_coordinate;
	int status = 0;//在己方camp0，在公共区域1，在对方camp2
	PieceColor color = UNKNOWN_COLOR;
};

class Camps {
public:
	Camps() = default;
	Camps(PieceColor a):color(a) {
		if (a == BLACK_P) {
			for (int i = 0; i < 5; i++)
			{
                this->left_border.push_back(0);
			}
			this->right_border.push_back(4);
			this->right_border.push_back(4);
			this->right_border.push_back(3);
			this->right_border.push_back(2);
			this->right_border.push_back(1);
		}
		else if (a == WHITE_P) {
			for (int i = 0; i < 5; i++)
			{
				this->right_border.push_back(15);
			}
			this->left_border.push_back(14);
			this->left_border.push_back(13);
			this->left_border.push_back(12);
			this->left_border.push_back(11);
			this->left_border.push_back(11);
		}
	}
	PieceColor Getcolor() {
		return this->color;
	}
	int GetLeftBorder(int r) {
		return (this->left_border)[r];
	}
	int GetRightBorder(int r) {
		return (this->right_border)[r];
	}
	void SetColor(PieceColor a) {
		this->color = a;
	}
	bool WhetherInCamps(int r,int c) {
		if (this->color == WHITE_P) {
			if (r - 11 < 0)
				return false;
			else
			{
				if ((c >= (this->left_border)[r - 11]) && (c <= (this->right_border)[r - 11]))
					return true;
				else
				{
					return false;
				}
			}
		}
		else if (this->color==BLACK_P)
		{
			if (r>4)
				return false;
			else
			{
				if ((c >= (this->left_border)[r]) && (c <= (this->right_border)[r]))
					return true;
				else
				{
					return false;
				}
			}
		}
	}
	void ShowCamps(ostream& cout) {
		cout << "color:" << this->color<<" ";
		for (int i = 0; i < 5; i++)
		{
			cout << this->left_border[i] << " "<<this->right_border[i]<<" ";
		}
		cout << endl;
	}
private:
	PieceColor color = UNKNOWN_COLOR;
	vector<int> left_border;
	vector<int> right_border;
};

class Board {
public:
	Board() {
		this->board_statuses = vector<vector<BoardStatus>>(16, vector<BoardStatus>(16, EMPTY));
	}
	void SetBoard(int r,int c,char grid) {
		if (grid == 'W') {
			this->board_statuses[r][c] = WHITE;
		}
		else if (grid == 'B') {
			this->board_statuses[r][c] = BLACK;
		}
		else if (grid=='.')
		{
			this->board_statuses[r][c] = EMPTY;
		}
	}
	BoardStatus GetBoardGrid(int r,int c) {
		return board_statuses[r][c];
	}
	void UpdateBoard() {

	}
	void ShowBoard(ostream &cout) {
		for (int r = 0; r < 16; r++)
		{
			for (int c = 0; c < 16; c++)
			{
				if (this->board_statuses[r][c] == WHITE)
					cout << 'W';
				else if (this->board_statuses[r][c] == BLACK)
					cout << 'B';
				else
				{
					cout << '.';
				}
			}
			cout << endl;
		}
		
	}
private:
	vector<vector<BoardStatus>> board_statuses;
};

class Action {
public:
	Action() = default;
	Action(int i,int r1,int c1,int r2,int c2){
		this->piece_index = i;
		this->start_position.r = r1;
		this->start_position.c = c1;
		this->end_position.r = r2;
		this->end_position.c = c2;
	}
	int GetIndex() {
		return this->piece_index;
	}
	Coordinate GetStartCoordinate() {
		return start_position;
	}
	Coordinate GetEndCoordinate() {
		return end_position;
	}
	ActionType ShowType() {
		return this->action_type;
	}
	void ShowAction() {
		cout <<"Type:"<<this->action_type<<" 棋子:"<<this->piece_index<<" 起点" << this->GetStartCoordinate().r << ',' << this->GetStartCoordinate().c << " 终点" << this->GetEndCoordinate().r << ',' << this->GetEndCoordinate().c <<' ';
		if (this->action_type == JUMP)
		{
			this->ShowPath();
		}
		cout << endl;
	}
	virtual void ShowPath() {

	}
	virtual vector<Coordinate> GetPath() {
		return apath;
	}
	void SetDepth(int a) {
		this->depth = a;
	}
protected:
	ActionType action_type=UNKNOWN_ACTION;
	int piece_index=0;//移动的棋子在player的player_piece数组中的下标
	Coordinate start_position;
	Coordinate end_position;
	int depth = 0;
	vector<Coordinate> apath;
};

class ActionMove :public Action{
public:
	ActionMove() = default;
	ActionMove(int i, int r1, int c1, int r2, int c2) {
		this->piece_index = i;
		this->start_position.r = r1;
		this->start_position.c = c1;
		this->end_position.r = r2;
		this->end_position.c = c2;
		this->action_type = MOVE;
	}
private:
	
};

class ActionJump :public Action {
public:
	ActionJump() = default;
	ActionJump(int i, int r1, int c1, int r2, int c2) {
		this->piece_index = i;
		this->start_position.r = r1;
		this->start_position.c = c1;
		this->end_position.r = r2;
		this->end_position.c = c2;
		this->action_type = JUMP;
	}
	virtual vector<Coordinate> GetPath(){
		return this->path;
	}
	void SetPath(int r,int c) {
		path.push_back(Coordinate(r, c));
	}
	virtual void ShowPath() {
		cout << " Path:";
		for (int i = 0; i < this->path.size(); i++)
		{
			cout << path[i].r << ',' << path[i].c << ' ';
		}
		cout << endl;
	}
private:
	vector<Coordinate> path;//跳跃路径
};

class Player {
public:
	Player() = default;
	Player(PieceColor a) {
		char color;
		if (a == WHITE_P) {
			this->player_color = WHITE_P;
			this->player_camp = Camps(a);
			this->aim_camp = Camps(BLACK_P);			
			color = 'W';
		}
		else if (a==BLACK_P)
		{
			this->player_color = BLACK_P;
			this->player_camp = Camps(a);
			this->aim_camp = Camps(WHITE_P);
			color = 'B';
		}
	}
	void PushPieces(int r,int c) {
		int temp_status;
		if ((this->player_camp).WhetherInCamps(r,c) ){
			temp_status = 0;
		}
		else if ((this->aim_camp).WhetherInCamps(r,c))
		{
			temp_status = 2;
		}
		else
		{
			temp_status = 1;
		}
		Piece temp = Piece(r, c, this->player_color, temp_status);
		this->player_piece.push_back(temp);
	}
	void ShowPlayer(ostream& cout) {
		cout <<"Player Color: " <<this->player_color << endl;
		cout << "Pieces:" << endl;
		for (int i = 0; i < 19; i++)
		{
			(this->player_piece)[i].ShowPiece(cout);
		}
		cout << "player_camp:" << endl;
		(this->player_camp).ShowCamps(cout);
		cout << "aim_camp:" << endl;
		(this->aim_camp).ShowCamps(cout);
	}
	vector<Action*> Expand(Board &board) {//expand没有改变棋子的坐标，只是举出了当前状态下所有棋子下一步走向的所有可能
		vector<Action*> expand_actions;
		vector<int> indexes = IsInCamps();
		if (indexes.empty())
		{
			return this->OutOfCampsGenerateAction(board);
		}
		else
		{
			// TODO
			expand_actions = this->InCampsGenerateAction(indexes, board); 
			if(expand_actions.size()>0)
				return expand_actions;
			else {
				return this->OutOfCampsGenerateAction(board);
			}
		}
	}
	vector<int> IsInCamps() {
		vector<int> incamp_pieces;
		if (player_piece.size() > 19) {
			for (int i = 0; i < player_piece.size(); i++)
			{
				player_piece[i].ShowPiece(cout);
			}
			cout << player_piece.size()<<endl;
			this->ShowPlayer(cout);
			exit(0);
		}
		for (int i = 0; i < player_piece.size(); i++)
		{
			if (player_camp.WhetherInCamps(player_piece[i].GetCoordinate().r, player_piece[i].GetCoordinate().c))
				incamp_pieces.push_back(i);
		}
		return incamp_pieces;
	}
	vector<Action*> InCampsGenerateAction(vector<int> indexes, Board& board) {
		vector<Action*> action_list;//能出营的action和不能出营但是远离corner的action放一块了
		vector<Action*> out_campus;//出营
		vector<Action*> in_campus;//营内活动
		unordered_map<Action*, bool> action_map;
		for (int i = 0; i < indexes.size(); i++)
		{
			vector<vector<bool>> is_visited = vector<vector<bool>>(16, vector<bool>(16, false));
			for (int j = 0; j < 8; j++)//添加move acion
			{
				int old_piece_r = player_piece[indexes[i]].GetCoordinate().r;
				int old_piece_c = player_piece[indexes[i]].GetCoordinate().c;
				int new_piece_r = player_piece[indexes[i]].GetCoordinate().r+direction_vector_row[j];
				int new_piece_c = player_piece[indexes[i]].GetCoordinate().c+direction_vector_column[j];
				if ((new_piece_r>=0)&&(new_piece_r<=15)&&(new_piece_c>=0)&&(new_piece_c<=15)) {
					if (board.GetBoardGrid(new_piece_r, new_piece_c) == EMPTY)
					{
						ActionMove* temp = new ActionMove(indexes[i], old_piece_r, old_piece_c, new_piece_r, new_piece_c);
						if (IsValid((Action*)temp) && IsAwayPlayerCamp(temp))//待添加判断是否远离自己的阵营
						{
							    action_list.push_back(temp);
								is_visited[new_piece_r][new_piece_c] = true;
						}
						else
						{
							delete temp;
						}
					}
				}
			}
			vector<Coordinate> path;
			path.push_back(player_piece[indexes[i]].GetCoordinate());
			is_visited[player_piece[i].GetCoordinate().r][player_piece[i].GetCoordinate().c] = true;
			DFSGetJump(player_piece[indexes[i]].GetCoordinate(), indexes[i],action_list, board,is_visited,path);//添加jump action
			int bk = 0;
		}
		for (int i = 0; i < action_list.size(); i++)
		{
			/*Action* temp = new Action(action_list[i]->GetIndex(), action_list[i]->GetStartCoordinate().r, action_list[i]->GetStartCoordinate().c, action_list[i]->GetEndCoordinate().r, action_list[i]->GetEndCoordinate().c);*/
			if ((player_camp.WhetherInCamps(action_list[i]->GetStartCoordinate().r, action_list[i]->GetStartCoordinate().c)) && (!player_camp.WhetherInCamps(action_list[i]->GetEndCoordinate().r, action_list[i]->GetEndCoordinate().c)))
				out_campus.push_back(action_list[i]);
		}
		if (out_campus.size() > 0)
			return out_campus;
		return action_list;
	}
	vector<Action*> OutOfCampsGenerateAction(Board& board) {
		vector<Action*> action_list;
		for (int i = 0; i < player_piece.size(); i++)
		{
			vector<vector<bool>> is_visited = vector<vector<bool>>(16, vector<bool>(16, false));
			for (int j = 0; j < 8; j++)
			{
				int old_piece_r = player_piece[i].GetCoordinate().r;
				int old_piece_c = player_piece[i].GetCoordinate().c;
				int new_piece_r = player_piece[i].GetCoordinate().r + direction_vector_row[j];
				int new_piece_c = player_piece[i].GetCoordinate().c + direction_vector_column[j];
				if ((new_piece_r >= 0) && (new_piece_r <= 15) && (new_piece_c >= 0) && (new_piece_c <= 15)) {
					if (board.GetBoardGrid(new_piece_r, new_piece_c) == EMPTY)
					{
						ActionMove* temp = new ActionMove(i, old_piece_r, old_piece_c, new_piece_r, new_piece_c);
						if (IsValid((Action*)temp) && IsAwayPlayerCamp(temp))//待添加判断是否远离自己的阵营
						{
							is_visited[new_piece_r][new_piece_c] = true;
								action_list.push_back(temp);
						}
							
						else
						{
							delete temp;
						}
					}
				}
			}
			vector<Coordinate> path;
			path.push_back(player_piece[i].GetCoordinate());
			is_visited[player_piece[i].GetCoordinate().r][player_piece[i].GetCoordinate().c] = true;
			DFSGetJump(player_piece[i].GetCoordinate(), i, action_list, board, is_visited, path);//添加jump action
			int bk = 0;
		}
		return action_list;
	}
	bool IsValid(Action* a) {//通用规则，不能跳回己方阵营，在对方阵营的棋子不能跳出对方阵营
		int index = a->GetIndex();
		bool isvalid1;
		bool isvalid2;
		isvalid1 = ((!(aim_camp.WhetherInCamps(a->GetStartCoordinate().r,a->GetStartCoordinate().c))) || (aim_camp.WhetherInCamps(a->GetEndCoordinate().r, a->GetEndCoordinate().c)));
		isvalid2 = ((player_camp.WhetherInCamps(a->GetStartCoordinate().r,a->GetStartCoordinate().c)) || (!(player_camp.WhetherInCamps(a->GetEndCoordinate().r, a->GetEndCoordinate().c))));
		return (isvalid1 && isvalid2);//起点在己方营地为真
	}
	bool IsAwayPlayerCamp(Action* a) {//判断在己方营地内move时是否做到远离营地//出营合法性
		int camps_r=0;//己方营地最里面的点的坐标:(0,0)或(15,15)
		int camps_c=0;
		if (player_camp.Getcolor() == WHITE_P) {
			camps_r = 15;
			camps_c = 15;
		}
		else if (player_camp.Getcolor() == BLACK_P) {
			camps_r = 0;
			camps_c = 0;
		}
		if (!player_camp.WhetherInCamps(a->GetStartCoordinate().r, a->GetStartCoordinate().c)) {//若起点就不在己方营地，返回true
			return true;
		}
		else if(player_camp.WhetherInCamps(a->GetStartCoordinate().r, a->GetStartCoordinate().c)){//起点在己方营地
			if (!player_camp.WhetherInCamps(a->GetEndCoordinate().r, a->GetEndCoordinate().c))//终点出营
				return true;
			else//终点不出营，判断是否做到远离最里面的点
			{
				int old_distance = abs(a->GetStartCoordinate().r - camps_r) + abs(a->GetStartCoordinate().c - camps_c);
				int new_distance = abs(a->GetEndCoordinate().r - camps_r) + abs(a->GetEndCoordinate().c - camps_c);
				if (new_distance > old_distance)
					return true;
				else
				{
					return false;
				}
			}
		}
	}
	void DFSGetJump(Coordinate piece_coor, int index, vector<Action*>& action_list, Board& board, vector<vector<bool>>& is_visited,vector<Coordinate> &path) {//push连跳action
		for (int i = 0; i < 8; i++)//8个方向向量
		{
			int old_piece_r = piece_coor.r;
			int old_piece_c = piece_coor.c;
			int new_piece_r = old_piece_r + direction_vector_row[i];
			int new_piece_c = old_piece_c + direction_vector_column[i];
			if ((new_piece_r>=0)&&(new_piece_r<=15)&&(new_piece_c>=0)&&(new_piece_c<=15)&&board.GetBoardGrid(new_piece_r, new_piece_c) != EMPTY) {//要移动的棋子的邻居有没有棋子，有才能进一步判断能不能跳
				int plan_jump_r = new_piece_r+direction_vector_row[i];
				int plan_jump_c = new_piece_c + direction_vector_column[i];
				//再判断预计落地点是否能跳
				if ((plan_jump_r>=0) && (plan_jump_r<=15) && (plan_jump_c>=0) && (plan_jump_c<=15)) {//不越界
					if (board.GetBoardGrid(plan_jump_r, plan_jump_c) == EMPTY&&(!is_visited[plan_jump_r][plan_jump_c])) {//落地点为空且未访问过
						ActionJump* temp = new ActionJump(index, path[0].r, path[0].c, plan_jump_r, plan_jump_c);
						path.push_back(Coordinate(plan_jump_r, plan_jump_c));
						for (int i = 0; i < path.size(); i++)
						{
							temp->SetPath(path[i].r, path[i].c);//
						}
						if (IsAwayPlayerCamp(temp)&&(IsValid(temp))) {
							action_list.push_back(temp);
							is_visited[plan_jump_r][plan_jump_c] = true;
							DFSGetJump(Coordinate(plan_jump_r,plan_jump_c), index, action_list, board, is_visited,path);
						}
						else {
							is_visited[plan_jump_r][plan_jump_c] = true;//不能省去，否则会反复横跳
							DFSGetJump(Coordinate(plan_jump_r, plan_jump_c), index, action_list, board, is_visited, path);
						}

						path.pop_back();
					}
				}
			}
		}
	}
	void SetPiece(Action* a) {
		player_piece[a->GetIndex()].SetCoordinate(a->GetEndCoordinate().r, a->GetEndCoordinate().c);
		if (player_camp.WhetherInCamps(a->GetEndCoordinate().r, a->GetEndCoordinate().c))
			player_piece[a->GetIndex()].SetStatus(0);
		else if (aim_camp.WhetherInCamps(a->GetEndCoordinate().r, a->GetEndCoordinate().c))
		{
			player_piece[a->GetIndex()].SetStatus(2);
		}
		else
		{
			player_piece[a->GetIndex()].SetStatus(1);
		}
	}
	void ReversePiece(Action* a) {
		player_piece[a->GetIndex()].SetCoordinate(a->GetStartCoordinate().r, a->GetStartCoordinate().c);
		if (player_camp.WhetherInCamps(a->GetStartCoordinate().r, a->GetStartCoordinate().c))
			player_piece[a->GetIndex()].SetStatus(0);
		else if (aim_camp.WhetherInCamps(a->GetStartCoordinate().r,a->GetStartCoordinate().c))
		{
			player_piece[a->GetIndex()].SetStatus(2);
		}
		else
		{
			player_piece[a->GetIndex()].SetStatus(1);
		}
	}
	Piece GetPiece(int i) {
		return this->player_piece[i];
	}
	PieceColor GetPlayerColor() {
		return this->player_color;
	}
	int GetAimCampusCoor() {
		if (this->player_color == WHITE_P)
			return 0;
		else
		{
			return 15;
		}
	}
	void ShowActionList(vector<Action*> action_list) {
		for (size_t i = 0; i < action_list.size(); i++)
		{
			action_list[i]->ShowAction();
		}
	}
private:
	PieceColor player_color=UNKNOWN_COLOR;
	vector<Piece> player_piece;
	Camps player_camp;
	Camps aim_camp;
};

class AlphaBetaPrune {
public:
	AlphaBetaPrune() = default;	
	AlphaBetaPrune(Player &player_user, Player &player_opponet,Board &board) {
		this->player_max = player_user;
		this->player_min = player_opponet;
		this->chess_board = board;
		this->depth = 2;
	}
	void ShowPrune(ostream& cout) {
		this->player_max.ShowPlayer(cout);
		this->player_min.ShowPlayer(cout);
		this->chess_board.ShowBoard(cout);
	}
	void AlphaBetaSearch(int dp) {
		double alpha = -10000;
		double beta = INT_MAX;
		this->depth = dp;
		double prev_delta = 0.0;
		int num = 0;
		for (int i = 0; i < 19; i++)
		{
			if (this->player_max.GetPiece(i).GetStatus() == 2)
				num++;
		}
		if (num > 15)
			this->depth=3;
		MaxValue(alpha, beta, this->depth,prev_delta);
		Result(optimal_action,player_max,chess_board);
		Savefile();
		SaveFileOutput(optimal_action);
	}
	int Evaluation(Player &player_max,Player &player_min,Board &board) {//Evaluation(己方,对方,棋盘)
		int eval = 0;
		int distance_op = 0;
		int distance = 0;
		int invasion_num = 0;
		int invasion_op_num = 0;
		for (int i = 0; i < 19; i++)
		{
			if (player_max.GetPiece(i).GetStatus() == 2) {
				eval = eval + 100;
				invasion_num++;
			}
			if (player_min.GetPiece(i).GetStatus() == 2) {
				invasion_op_num++;
			}
			if (player_min.GetPlayerColor() == WHITE_P) {
				distance_op = player_min.GetPiece(i).GetCoordinate().r + player_min.GetPiece(i).GetCoordinate().c;
				distance = abs(player_max.GetPiece(i).GetCoordinate().r + player_max.GetPiece(i).GetCoordinate().c - 30);
				eval = eval + distance_op - distance;
			}
			else
			{
				distance_op = abs(player_min.GetPiece(i).GetCoordinate().r + player_min.GetPiece(i).GetCoordinate().c-30);
				distance = player_max.GetPiece(i).GetCoordinate().r + player_max.GetPiece(i).GetCoordinate().c;
				eval = eval + distance_op - distance;
			}
		}
		if ((invasion_num == 19) || (invasion_op_num == 19))
			eval = 5000;
		return eval;
	}
	double MaxValue(double alpha,double beta,int depth,double prev) {
		double eval = 0.0;
		if(depth==this->depth)
			double eval= Evaluation(player_max, player_min, chess_board);
		double prev_delta=0.0;
		if (depth == 0|| Evaluation(player_max, player_min, chess_board) ==5000)
			return eval+prev;
		double v = -10000;
		double a;
		auto max_actions=player_max.Expand(chess_board);
		for (int i = 0; i < max_actions.size(); i++)
		{
			int previous_status = player_max.GetPiece(max_actions[i]->GetIndex()).GetStatus();
			Result(max_actions[i],player_max,chess_board);
			double temp = 0.0;
			Coordinate end = max_actions[i]->GetEndCoordinate();
			Coordinate start = max_actions[i]->GetStartCoordinate();
			if (previous_status!=2&&player_max.GetPiece(max_actions[i]->GetIndex()).GetStatus() == 2)
				temp += 10;
			int coor = player_max.GetAimCampusCoor();
			if (coor == 0)
				temp = temp - (double)(end.r + end.c - start.r - start.c);
			else
			{
				temp = temp - (double)(start.r + start.c - end.r - end.c);
			}
			prev_delta = temp * (double)depth / 10000.0 + prev;
			v = DoubleMax(v, MinValue(alpha, beta, depth-1,prev_delta+eval));
			Restore(max_actions[i],player_max,chess_board);
			if (v >= beta) {
                return v;
			}			
			a = DoubleMax(alpha, v);
			if (alpha != a&&depth==this->depth) {//必须在alpha更新且在顶层的MaxValue里才能设置optimal_action
				this->optimal_action = max_actions[i];
				optimal_action->SetDepth(depth);
			}
			alpha = a;
		}
		return v;
	}
	double MinValue(double alpha, double beta,int depth,double prev) {
		double eval = 0.0;
		double prev_delta=0.0;
		if (depth==0|| Evaluation(player_max, player_min, chess_board) ==5000)
		{
			return eval+prev;
		}
		double v = INT_MAX;
		auto min_action = player_min.Expand(chess_board);
		for (int i = 0; i < min_action.size(); i++)
		{
			int previous_status = player_min.GetPiece(min_action[i]->GetIndex()).GetStatus();
			Result(min_action[i],player_min,chess_board);
			double temp = 0.0;
			Coordinate end = min_action[i]->GetEndCoordinate();
			Coordinate start = min_action[i]->GetStartCoordinate();
			if (previous_status!=2&&player_min.GetPiece(min_action[i]->GetIndex()).GetStatus() == 2)
				temp += 10;
			int coor = player_min.GetAimCampusCoor();
			if (coor == 0)
				temp = temp - (end.r + end.c - start.r - start.c);
			else
			{
				temp = temp - (start.r + start.c - end.r - end.c);
			}
			prev_delta = prev - temp * (double)depth / 10000.0;
			v = DoubleMin(v, MaxValue(alpha, beta, depth-1,prev_delta+eval));
			Restore(min_action[i],player_min,chess_board);
			if (v <= alpha)
				return v;
			beta = DoubleMin(beta, v);
		}
		return v;
	}
	void Result(Action* a,Player &player,Board &board) {
		player.SetPiece(a);
		board.SetBoard(a->GetStartCoordinate().r,a->GetStartCoordinate().c,'.');//action的起点变为空
		if (player.GetPlayerColor() == WHITE_P) {
            board.SetBoard(a->GetEndCoordinate().r,a->GetEndCoordinate().c,'W');//action的终点变为对应的棋子
		}           
		else
		{
			board.SetBoard(a->GetEndCoordinate().r, a->GetEndCoordinate().c, 'B');
		}		
	}
	void Restore(Action* a,Player &player,Board &board) {
		player.ReversePiece(a);
		board.SetBoard(a->GetEndCoordinate().r, a->GetEndCoordinate().c, '.');//action的终点变为空
		if (player.GetPlayerColor() == WHITE_P) {
            board.SetBoard(a->GetStartCoordinate().r, a->GetStartCoordinate().c, 'W');//action的起点变为对应的棋子
		}			
		else
		{
			board.SetBoard(a->GetStartCoordinate().r, a->GetStartCoordinate().c, 'B');
		}
	}
	void ShowABBoard() {
		this->chess_board.ShowBoard(cout);
		cout << endl;
	}
	void Savefile() {
		ofstream savefile("input.txt");
		savefile << "GAME" << '\n';
		if (this->player_max.GetPlayerColor() == WHITE_P)
			savefile << "BLACK" << '\n';
		else
		{
			savefile << "WHITE" << '\n';
		}
		savefile << "100.0" << '\n';
		for (int i = 0; i < 16; i++)
		{
			for (int j = 0; j < 16; j++)
			{
				if (this->chess_board.GetBoardGrid(i, j) == BLACK)
					savefile << 'B';
				else if (this->chess_board.GetBoardGrid(i,j)==WHITE)
				{
					savefile << 'W';
				}
				else
				{
					savefile << '.';
				}
			}
			if (i < 15)
				savefile << '\n';
		}
	}
	void SaveFileOutput(Action* a) {
		ofstream savefile("output.txt");
		if (a->ShowType() == MOVE) {
			savefile << 'E'<<' ';
			savefile << a->GetStartCoordinate().c << ',' << a->GetStartCoordinate().r<<' ';
			savefile << a->GetEndCoordinate().c << ',' << a->GetEndCoordinate().r;
			if (abs(a->GetStartCoordinate().c - a->GetEndCoordinate().c) > 1 || abs(a->GetStartCoordinate().r - a->GetEndCoordinate().r) > 1)
				exit(0);
		}
		else
		{
			vector<Coordinate> jump_path = a->GetPath();
			for (int i = 0; i < jump_path.size()-1; i++)
			{
				savefile << 'J' << ' ';
				savefile << jump_path[i].c << ',' << jump_path[i].r<<' ';
				savefile << jump_path[i + 1].c << ',' << jump_path[i + 1].r ;
				if (abs(jump_path[i].c - jump_path[i + 1].c) > 2 || abs(jump_path[i].r - jump_path[i + 1].r) > 2)
					exit(0);
				if (i<jump_path.size()-2)
				{
					savefile << '\n';
				}
			}
		}
	}
private:
	int depth=2;
	string GameMode;
	string color_of_player;
	float remaining_time;
	Player player_max;//玩家自己
	Player player_min;//对手
	Board chess_board;
	vector<Action*> actions;
	Action* optimal_action;
};
//test
void push(vector<int> a)
{
	a.push_back(3);
	cout << a.size() << endl;
	for (int i = 0; i < a.size(); i++)
	{
		cout << a[i] << " ";
	}
	cout << endl;
	if (a.size() < 10)
		push(a);
};
int main()
{
	long total_time = 0;
	int black_steps = 0;
	int white_steps = 0;
	long black_time = 0;
	long white_time = 0;
  	while (true) {
		clock_t start, end;
		start = clock();
		vector<string> original_board;//存储input读取的board
		ifstream infile("input.txt");
		string gamemode, color, time;
		double remain_time=0.0;
		PieceColor piece_color_of_player = UNKNOWN_COLOR;
		char player_char = ' ';
		PieceColor piece_color_of_opponent = UNKNOWN_COLOR;
		char opponent_char = ' ';
		Board game_board;//棋盘
		string line;
		int i = 0;//文件第i行
		while (getline(infile, line)) {
			line = line.erase(line.find_last_not_of('\r') + 1);//针对UNIX系统getline会有行尾空格
			if (i == 0) {
				gamemode = line;
			}
			else if (i == 1) {
				color = line;
				if (color == "BLACK") {
					piece_color_of_player = BLACK_P;
					player_char = 'B';
					piece_color_of_opponent = WHITE_P;
					opponent_char = 'W';
				}
				else if (color == "WHITE") {
					piece_color_of_player = WHITE_P;
					player_char = 'W';
					piece_color_of_opponent = BLACK_P;
					opponent_char = 'B';
				}
			}
			else if (i == 2) {
				time = line;
				remain_time = stod(time);
			}
			else
			{
				original_board.push_back(line);
			}
			i++;
		}
		if (color == "BLACK")
			black_steps++;
		else
		{
			white_steps++;
		}
		infile.close();
		//读取文件
		Player player_user = Player(piece_color_of_player);//玩家自己
		Player player_opponent = Player(piece_color_of_opponent);//对手
		for (int r = 0; r < 16; r++) {
			for (int c = 0; c < 16; c++) {
				game_board.SetBoard(r, c, original_board[r][c]);//设置棋盘
				if (original_board[r][c] == player_char) {
					player_user.PushPieces(r, c);
				}
				else if (original_board[r][c] == opponent_char)
				{
					player_opponent.PushPieces(r, c);
				}
			}
		}
		AlphaBetaPrune alpha_beta_pruning(player_user, player_opponent, game_board);
		int depth = 2;
		if (remain_time < 15.0)
			depth = 1;
		alpha_beta_pruning.AlphaBetaSearch(depth);
		end = clock();
		alpha_beta_pruning.ShowABBoard();
		cout << "运行时间：" << (float)(end - start) * 1000 /CLOCKS_PER_SEC << " ms" << endl;
		if (color == "BLACK") {
			black_time += (float)(end - start) * 1000 / CLOCKS_PER_SEC;
			cout << "黑方总时间：" << black_time << " ms" << endl;
		}
		else
		{
			white_time += (float)(end - start) * 1000 / CLOCKS_PER_SEC;
			cout << "白方总时间：" << white_time << " ms" << endl;
		}
		cout << "黑方步数: " << black_steps << " 白方步数: " << white_steps << endl;
	}
}
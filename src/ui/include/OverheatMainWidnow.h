#include <QMainWindow>

class OverheatMainWindow : public QMainWindow {
public:
	static OverheatMainWindow& instance() { static OverheatMainWindow inst; return inst; }
private:
	OverheatMainWindow() = default;
};

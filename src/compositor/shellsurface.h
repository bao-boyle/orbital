
#ifndef ORBITAL_SHELLSURFACE_H
#define ORBITAL_SHELLSURFACE_H

#include <functional>

#include <QHash>
#include <QObject>

#include "interface.h"
#include "utils.h"

struct wl_client;
struct weston_surface;
struct weston_shell_client;

namespace Orbital
{

class Shell;
class ShellView;
class Workspace;
class Output;
class Seat;

class ShellSurface : public Object
{
    Q_OBJECT
public:
    ShellSurface(Shell *shell, weston_surface *surface);
    ~ShellSurface();

    typedef std::function<void (weston_surface *, int, int)> ConfigureSender;
    enum class Type {
        None = 0,
        Toplevel = 1,
        Popup = 2
    };

    ShellView *viewForOutput(Output *o);
    bool isMapped() const;
    void setWorkspace(Workspace *ws);
    Workspace *workspace() const;
    wl_client *client() const;

    void setConfigureSender(ConfigureSender sender);
    void setToplevel();
    void setPopup(weston_surface *parent, Seat *seat, int x, int y);
    void setMaximized();
    void move(Seat *seat);

    static ShellSurface *fromSurface(weston_surface *s);

signals:
    void popupDone();

private:
    static void staticConfigure(weston_surface *s, int x, int y);
    void configure(int x, int y);
    void updateState();

    Shell *m_shell;
    weston_surface *m_surface;
    std::function<void (weston_surface *, int, int)> m_configureSender;
    Workspace *m_workspace;
    QHash<int, ShellView *> m_views;

    Type m_type;
    Type m_nextType;

    weston_surface *m_parent;
    struct {
        int x;
        int y;
        Seat *seat;
    } m_popup;
    struct {
        bool maximized;
    } m_toplevel;
};

}

DECLARE_OPERATORS_FOR_FLAGS(Orbital::ShellSurface::State)

#endif
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

public class ComboEditor : EditorWindow
{
    List<BaseNode> windows = new List<BaseNode>();
    List<BaseNode> clearBackup = new List<BaseNode>();
    Vector2 mousePos;
    BaseNode selectedNode;
    ConnectionPoint selectedConnectionPoint;

    private GUIStyle inPointStyle;
    private GUIStyle outPointStyle;

    bool makeTransitionMode = false;


    private Vector2 offset;
    private Vector2 drag;

    //opens combo editor window
    [MenuItem("Window/Combo Editor")]
    static void ShowEditor()
    {
        ComboEditor editor = GetWindow<ComboEditor>();
        editor.titleContent = new GUIContent("Combo Editor");
    }

    private void OnGUI()
    {
        HandleEvents(Event.current);

        Draw();
    }

    void HandleEvents(Event _evt)
    {
        Event evt = _evt;
        mousePos = evt.mousePosition;

        switch (evt.type)
        {
            case EventType.MouseDrag:
                if (evt.button == 0)
                {
                    OnDrag(evt.delta);
                }
                break;
            case EventType.MouseDown:
                if (evt.button == 1 && !makeTransitionMode) // right click in editor window
                {
                    if (getClickedWindowIndex(mousePos) == -1) // on bg
                    {
                        GenericMenu menu = new GenericMenu();

                        menu.AddItem(new GUIContent("Add Input Node"), false, ContextCallback, "inputNode");
                        menu.AddItem(new GUIContent("Add Combo Node"), false, ContextCallback, "comboNode");
                        menu.AddSeparator("");
                        menu.AddItem(new GUIContent("Clear all nodes"), false, ContextCallback, "clear");
                        menu.AddItem(new GUIContent("Undo a previous clear"), false, ContextCallback, "undoClear");


                        menu.ShowAsContext();
                        evt.Use();
                    }
                    else // on a window
                    {
                        GenericMenu menu = new GenericMenu();

                        //menu.AddItem(new GUIContent("Make Transition"), false, ContextCallback, "makeTransition");
                        //menu.AddSeparator("");
                        menu.AddItem(new GUIContent("Delete Node"), false, ContextCallback, "deleteNode");

                        menu.ShowAsContext();
                        evt.Use();
                    }
                }
                else if (evt.button == 0) 
                {
                    if (makeTransitionMode) //left mouse when connecting two nodes
                    {
                        //int windowIndex = getClickedWindowIndex(mousePos);

                        //if (windowIndex > -1 && !windows[windowIndex].Equals(selectedNode)) //if clicked on a window
                        //{
                        //    float halfX = windows[windowIndex].windowRect.x + (windows[windowIndex].windowRect.width / 2);

                        //    bool isInput = mousePos.x <= halfX; // if mouse was clicked on left side of node it is input

                        //    selectedNode.MakeConnection(windows[windowIndex], false);
                        //    makeTransitionMode = false;
                        //    selectedNode = null;
                        //}
                        //else if (windowIndex <= -1) //click on bg, exit transition mode
                        //{
                        //    makeTransitionMode = false;
                        //    selectedNode = null;
                        //}

                        makeTransitionMode = false;
                        selectedConnectionPoint = null;

                        evt.Use();
                    }
                }
                //else if (evt.button == 0 && !makeTransitionMode) //enter make transition mode
                //{
                //    int windowIndex = getClickedWindowIndex(mousePos);

                //    if (windowIndex > -1)
                //    {
                //        windows[windowIndex].OnClick(mousePos);

                //        selectedNode = windows[windowIndex];

                //        makeTransitionMode = true;
                //    }
                //}
                break;
        }
        
    }

    void Draw()
    {
        if (makeTransitionMode && selectedNode != null)
        {
            Rect mouseRect = new Rect(mousePos.x, mousePos.y, 10, 10);

            DrawNodeCurve(selectedNode.windowRect, mouseRect, Color.blue);

            Repaint();
        }

        foreach (BaseNode node in windows)
        {
            node.inPoint.Draw();
            node.outPoint.Draw();
        }

        BeginWindows();

        for (int i = 0; i < windows.Count; ++i)
        {
            windows[i].windowRect = GUI.Window(i, windows[i].windowRect, DrawNodeWindow, windows[i].windowTitle);
        }

        EndWindows();
    }

    void DrawNodeWindow(int id)
    {
        windows[id].DrawWindow();

        GUI.DragWindow();
    }

    private void DrawGrid(float gridSpacing, float gridOpacity, Color gridColor)
    {
        int widthDivs = Mathf.CeilToInt(position.width / gridSpacing);
        int heightDivs = Mathf.CeilToInt(position.height / gridSpacing);

        Handles.BeginGUI();
        Handles.color = new Color(gridColor.r, gridColor.g, gridColor.b, gridOpacity);

        offset += drag * 0.5f;
        Vector3 newOffset = new Vector3(offset.x % gridSpacing, offset.y % gridSpacing, 0);

        for (int i = 0; i < widthDivs; i++)
        {
            Handles.DrawLine(new Vector3(gridSpacing * i, -gridSpacing, 0) + newOffset, new Vector3(gridSpacing * i, position.height, 0f) + newOffset);
        }

        for (int j = 0; j < heightDivs; j++)
        {
            Handles.DrawLine(new Vector3(-gridSpacing, gridSpacing * j, 0) + newOffset, new Vector3(position.width, gridSpacing * j, 0f) + newOffset);
        }

        Handles.color = Color.white;
        Handles.EndGUI();
    }

    private void OnDrag(Vector2 delta) //drag editor window
    {
        drag = delta;

        if (windows != null)
        {
            for (int i = 0; i < windows.Count; i++)
            {
                windows[i].Drag(delta);
            }
        }

        GUI.changed = true;
    }

    void ContextCallback(object _obj)
    {
        string callback = _obj.ToString();

        if(callback.Equals("inputNode"))
        {
            InputNode inputNode = new InputNode();
            inputNode.windowRect = new Rect(mousePos.x, mousePos.y, 200, 150);

            windows.Add(inputNode);
        }
        else if(callback.Equals("comboNode"))
        {
            ComboNode comboNode = new ComboNode();
            comboNode.windowRect = new Rect(mousePos.x, mousePos.y, 200, 100);

            windows.Add(comboNode);
        }
        else if (callback.Equals("makeTransition"))
        {
            bool clickedOnWindow = false;
            int selectIndex = -1;

            for (int i = 0; i < windows.Count; ++i)
            {
                if (windows[i].windowRect.Contains(mousePos))
                {
                    selectIndex = i;
                    clickedOnWindow = true;
                    break;
                }
            }

            if(clickedOnWindow)
            {
                selectedNode = windows[selectIndex];
                makeTransitionMode = true;
            }
        }
        else if (callback.Equals("deleteNode"))
        {
            bool clickedOnWindow = false;
            int selectIndex = -1;

            for (int i = 0; i < windows.Count; ++i)
            {
                if (windows[i].windowRect.Contains(mousePos))
                {
                    selectIndex = i;
                    clickedOnWindow = true;
                    break;
                }
            }

            if(clickedOnWindow)
            {
                BaseNode selNode = windows[selectIndex];
                windows.RemoveAt(selectIndex);

                foreach(BaseNode node in windows)
                {
                    node.NodeDeleted(selNode);
                }
            }
        }
        else if (callback.Equals("clear"))
        {
            clearNodes();
        }
        else if (callback.Equals("undoClear"))
        {
            undoClear();
        }
    }

    public static void DrawNodeCurve(Rect start, Rect end, Color color)
    {
        Vector3 startPos = new Vector3(start.x + start.width / 2, start.y + start.height / 2, 0);
        Vector3 endPos = new Vector3(end.x + end.width / 2, end.y + end.height / 2, 0);
        Vector3 startTan = startPos + Vector3.right * 50;
        Vector3 endtan = endPos + Vector3.left * 50;
        Color shadowCol = new Color(0, 0, 0, .06f);

        //draw shadow
        for(int i = 0; i < 3; ++i)
        {
            Handles.DrawBezier(startPos, endPos, startTan, endtan, shadowCol, null, (i + 1) * 5);
        }

        Handles.DrawBezier(startPos, endPos, startTan, endtan, color, null, 1);
    }

    //if this returns -1 no window was clicked
    int getClickedWindowIndex(Vector2 _mousePos)
    {
        int selectIndex = -1;

        for (int i = 0; i < windows.Count; ++i)
        {
            if (windows[i].windowRect.Contains(_mousePos))
            {
                selectIndex = i;
                break;
            }
        }

        return selectIndex;
    }

    void clearNodes()
    {
        clearBackup = windows;

        windows.Clear();
    }

    void undoClear()
    {
        if (clearBackup.Count > 0)
        {
            Debug.Log("works");
            windows = clearBackup;

            clearBackup.Clear();
        }
    }

    void OnConnectionPointClicked(ConnectionPoint _clickedPoint)
    {
        if (!makeTransitionMode)
        {
            makeTransitionMode = true;
            selectedConnectionPoint = _clickedPoint;
        }
        else
        {
            selectedConnectionPoint.MakeConnection(_clickedPoint);
            //makeTransitionMode = false;
            //selectedConnectionPoint = null;
        }
    }
}

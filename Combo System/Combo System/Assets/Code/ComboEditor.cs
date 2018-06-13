using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

public class ComboEditor : EditorWindow
{
   // List<BaseNode> windows = new List<BaseNode>();
    //List<BaseNode> clearBackup = new List<BaseNode>();
    Vector2 mousePos;
    BaseNode selectedNode;
    ConnectionPoint selectedConnectionPoint;

    int currentObjectIndex = 0;
    List<ComboEditorObject> editorObjects;
    string[] objectNames;

    private GUIStyle nodeStyle;
    private GUIStyle inPointStyle;
    private GUIStyle outPointStyle;

    bool makeTransitionMode = false;

    //private Vector2 offset;
    private Vector2 drag = Vector2.zero;

    List<Connection> deadConnections;

    //opens combo editor window
    [MenuItem("Window/Combo Editor")]
    static void ShowEditor()
    {
        ComboEditor editor = GetWindow<ComboEditor>();
        editor.titleContent = new GUIContent("Combo Editor");
    }

    private void OnEnable()
    {
        Reload();

        nodeStyle = new GUIStyle();
        nodeStyle.normal.background = EditorGUIUtility.Load("builtin skins/darkskin/images/node1.png") as Texture2D;
        nodeStyle.border = new RectOffset(12, 12, 12, 12);

        inPointStyle = new GUIStyle();
        inPointStyle.normal.background = EditorGUIUtility.Load("builtin skins/darkskin/images/btn left.png") as Texture2D;
        inPointStyle.active.background = EditorGUIUtility.Load("builtin skins/darkskin/images/btn left on.png") as Texture2D;
        inPointStyle.border = new RectOffset(4, 4, 12, 12);

        outPointStyle = new GUIStyle();
        outPointStyle.normal.background = EditorGUIUtility.Load("builtin skins/darkskin/images/btn right.png") as Texture2D;
        outPointStyle.active.background = EditorGUIUtility.Load("builtin skins/darkskin/images/btn right on.png") as Texture2D;
        outPointStyle.border = new RectOffset(4, 4, 12, 12);
    }

    private void OnGUI()
    {
        //Reload();

        if (editorObjects.Count > 0)//if there are objects to edit
        {
            HandleEvents(Event.current);

            Draw();
        }
        else //if there are not objects to edit draws the dropdown to select one
            currentObjectIndex = EditorGUILayout.Popup(currentObjectIndex, objectNames);

        ClearDeadConnections();

        if (GUI.changed) Repaint();
    }

    void HandleEvents(Event _evt)
    {
        Event evt = _evt;
        mousePos = evt.mousePosition;
        drag = Vector2.zero;

        switch (evt.type)
        {
            case EventType.MouseDrag:
                if (evt.button == 0 && !selectedNode)
                {
                    OnDrag(evt.delta);
                }
                break;
            case EventType.MouseDown:
                if (evt.button == 1 && !makeTransitionMode) // right click on bg
                {
                    if (getClickedWindowIndex(mousePos) == -1) // on bg
                    {
                        GenericMenu menu = new GenericMenu();

                        menu.AddItem(new GUIContent("Add Input Node"), false, ContextCallback, "inputNode");
                        menu.AddItem(new GUIContent("Add Combo Node"), false, ContextCallback, "comboNode");
                        menu.AddSeparator("");
                        menu.AddItem(new GUIContent("Clear all nodes"), false, ContextCallback, "clear");
                        menu.AddItem(new GUIContent("Reload editor"), false, ContextCallback, "reload");


                        menu.ShowAsContext();
                        evt.Use();
                    }
                    else // right click on a node
                    {
                        GenericMenu menu = new GenericMenu();

                        menu.AddItem(new GUIContent("Delete Node"), false, ContextCallback, "deleteNode");

                        menu.ShowAsContext();
                        evt.Use();
                    }
                }
                else if (evt.button == 0) 
                {
                    int clickedWindowIndex = getClickedWindowIndex(mousePos);

                    if (clickedWindowIndex > -1)
                    {
                        selectedNode = editorObjects[currentObjectIndex].windows[clickedWindowIndex];
                    }
                    else
                    {
                        selectedNode = null;
                        selectedConnectionPoint = null;
                        makeTransitionMode = false;
                    }
                }

                //evt.Use();
                break;
        }
    }

    void Draw()
    {
        DrawGrid(20, 0.2f, Color.gray);
        DrawGrid(100, 0.4f, Color.gray);

        if (makeTransitionMode && selectedConnectionPoint != null)
        {
            Rect mouseRect = new Rect(mousePos.x, mousePos.y, 10, 10);

            DrawNodeCurve(selectedConnectionPoint.rect, mouseRect, Color.blue);

            Repaint();
        }


        foreach (BaseNode node in editorObjects[currentObjectIndex].windows)
        {
            if (node.inPoint == null || node.outPoint == null)
                Debug.Log("aaaaaa");
            else
                Debug.Log(node.windowRect.position);


            node.inPoint.Draw();
            node.outPoint.Draw();
        }

        foreach (Connection connection in editorObjects[currentObjectIndex].connections)
        {
            connection.Draw(Color.white);
        }

        BeginWindows();

        for (int i = 0; i < editorObjects[currentObjectIndex].windows.Count; ++i)
        {
            editorObjects[currentObjectIndex].windows[i].windowRect = GUI.Window(i, editorObjects[currentObjectIndex].windows[i].windowRect, DrawNodeWindow,
                editorObjects[currentObjectIndex].windows[i].windowTitle);
        }

        EndWindows();
        

        currentObjectIndex = EditorGUILayout.Popup(currentObjectIndex, objectNames);
    }

    void DrawNodeWindow(int id)
    {
        editorObjects[currentObjectIndex].windows[id].DrawWindow();

        GUI.DragWindow();
    }

    private void DrawGrid(float gridSpacing, float gridOpacity, Color gridColor)
    {
        int widthDivs = Mathf.CeilToInt(position.width / gridSpacing);
        int heightDivs = Mathf.CeilToInt(position.height / gridSpacing);

        Handles.BeginGUI();
        Handles.color = new Color(gridColor.r, gridColor.g, gridColor.b, gridOpacity);

        editorObjects[currentObjectIndex].offset += drag * 0.5f;
        Vector3 newOffset = new Vector3(editorObjects[currentObjectIndex].offset.x % gridSpacing, editorObjects[currentObjectIndex].offset.y % gridSpacing, 0);

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

        if (editorObjects[currentObjectIndex].windows != null)
        {
            for (int i = 0; i < editorObjects[currentObjectIndex].windows.Count; i++)
            {
                editorObjects[currentObjectIndex].windows[i].Drag(delta);
            }
        }

        GUI.changed = true;
    }

    void ContextCallback(object _obj)
    {
        string callback = _obj.ToString();

        if(callback.Equals("inputNode")) //make new input node
        {
            InputNode inputNode = new InputNode(mousePos, 200, 150, "Input Node", nodeStyle, inPointStyle, outPointStyle, OnConnectionPointClicked, OnConnectionPointClicked);

            editorObjects[currentObjectIndex].windows.Add(inputNode);
        }
        else if(callback.Equals("comboNode")) //make new combo node
        {
            ComboNode comboNode = new ComboNode(mousePos, 200, 100, "Combo Node", nodeStyle, inPointStyle, outPointStyle, OnConnectionPointClicked, OnConnectionPointClicked);

            editorObjects[currentObjectIndex].windows.Add(comboNode);
        }
        else if (callback.Equals("deleteNode"))
        {
            bool clickedOnWindow = false;
            int selectIndex = -1;

            for (int i = 0; i < editorObjects[currentObjectIndex].windows.Count; ++i)
            {
                if (editorObjects[currentObjectIndex].windows[i].windowRect.Contains(mousePos))
                {
                    selectIndex = i;
                    clickedOnWindow = true;
                    break;
                }
            }

            if(clickedOnWindow)
            {
                BaseNode selNode = editorObjects[currentObjectIndex].windows[selectIndex];
                editorObjects[currentObjectIndex].windows.RemoveAt(selectIndex);

                foreach(BaseNode node in editorObjects[currentObjectIndex].windows)
                {
                    node.NodeDeleted(selNode);
                }
            }
        }
        else if (callback.Equals("clear"))
        {
            clearNodes();
        }
        else if (callback.Equals("reload"))
        {
            Reload();
        }
        //else if (callback.Equals("undoClear"))
        //{
        //    undoClear();
        //}
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
        if (editorObjects.Count == 0)
            return -1;

        int selectIndex = -1;

        for (int i = 0; i < editorObjects[currentObjectIndex].windows.Count; ++i)
        {
            if (editorObjects[currentObjectIndex].windows[i].windowRect.Contains(_mousePos) 
                || editorObjects[currentObjectIndex].windows[i].inPoint.rect.Contains(_mousePos) 
                || editorObjects[currentObjectIndex].windows[i].outPoint.rect.Contains(_mousePos))
            {
                selectIndex = i;
                break;
            }
        }

        return selectIndex;
    }

    void clearNodes()
    {
        //clearBackup = windows;

        editorObjects[currentObjectIndex].windows.Clear();
        editorObjects[currentObjectIndex].connections.Clear();

    }

    //void undoClear()
    //{
    //    if (clearBackup.Count > 0)
    //    {
    //        Debug.Log("works");
    //        windows = clearBackup;

    //        clearBackup.Clear();
    //    }
    //}

    void OnConnectionPointClicked(ConnectionPoint _clickedPoint)
    {
        if (!makeTransitionMode)
        {
            makeTransitionMode = true;
            selectedConnectionPoint = _clickedPoint;
        }
        else
        {
            Connection newConnection = selectedConnectionPoint.MakeConnection(_clickedPoint, editorObjects[currentObjectIndex].connections.Count + 1, RemoveConnection);

            if (newConnection != null)
                editorObjects[currentObjectIndex].connections.Add(newConnection);

            makeTransitionMode = false;
            selectedConnectionPoint = null;
        }
    }

    void RemoveConnection(Connection _toRemove)
    {
        if (editorObjects.Count == 0)
            return;

        deadConnections.Add(_toRemove);
        //editorObjects[currentObjectIndex].connections.Remove(_toRemove);
    }

    void ClearDeadConnections()
    {
        foreach(Connection deadConnection in deadConnections)
        {
            editorObjects[currentObjectIndex].connections.Remove(deadConnection);
        }

        deadConnections.Clear();
    }

    void Reload()
    {
        //Init list for dropdown of editor objects
        editorObjects = new List<ComboEditorObject>();
        deadConnections = new List<Connection>();

        foreach (GameObject obj in GameObject.FindGameObjectsWithTag("ComboEditorObject"))
        {
            ComboEditorObject objectScript = obj.GetComponent<ComboEditorObject>();

            if (!objectScript)
                continue;

            objectScript.initIfNew();

            foreach(BaseNode node in objectScript.windows)
            {
                node.inPoint.OnClickConnectionPoint = OnConnectionPointClicked;
                node.outPoint.OnClickConnectionPoint = OnConnectionPointClicked;
            }

            foreach(Connection connection in objectScript.connections)
            {
                connection.onClickRemoveConnection = RemoveConnection;
            }

            editorObjects.Add(objectScript);
        }

        objectNames = new string[editorObjects.Count];

        for (int i = 0; i < editorObjects.Count; ++i)
        {
            objectNames[i] = editorObjects[i].gameObject.name;
        }
    }
}

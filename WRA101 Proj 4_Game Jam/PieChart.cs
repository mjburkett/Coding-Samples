using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class PieChart : MonoBehaviour{

    public Canvas cnvs;
    private TeammateManager mgr;
    private float[] values = new float[3];
    public Color[] wedgeColors;
    public Image wedgePrefab;
    public int teammateNum;

    void Start(){
        mgr = cnvs.GetComponent<TeammateManager>();
        for(int i = 0; i < values.Length; i++) {
            values[i] = mgr.getValues(i, teammateNum);
        }
        Debug.Log(values);
        MakeGraph();
    }

    void MakeGraph(){
        float total = 0f;
        float zRotation = 0f;
        for (int i = 0; i < values.Length; i++) {
            total += values[i];
        }
        for (int i = 0; i < values.Length; i++) {
            Image newWedge = Instantiate(wedgePrefab) as Image;
            newWedge.transform.SetParent(transform, false);
            newWedge.color = wedgeColors[i];
            newWedge.fillAmount = values[i] / total;
            newWedge.transform.rotation = Quaternion.Euler(new Vector3(0f, 0f, zRotation));
            zRotation -= newWedge.fillAmount * 360f;
        }
    }
}
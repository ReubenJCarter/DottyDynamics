using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor; 

namespace Dotty{
    public class Vortex : MonoBehaviour
    {
        private int internalId; 
        unsafe private VortexNtv* ptr; 
        
        public World world; 

        public float strength = 1; 
        public float minDistance = 0.1f; 
        public float maxDistance = 1000; 
        public Falloff falloff = Falloff.InvDist2; 


        // Start is called before the first frame update
        void Start()
        {
        }

        void OnEnable()
        {
            Vector3 position = transform.position; 
            Vector3 normal = transform.rotation * Vector3.up;
            internalId = world.AddVortex(position, normal, strength, minDistance, maxDistance, falloff); 
            unsafe{
                ptr = world.GetVortexPtr(internalId); 
            }
        }

        void OnDisable()
        {
            world.DestroyVortex(internalId); 
        }

        // Update is called once per frame
        void Update()
        {
            Vector3 position = transform.position; 
            Vector3 normal = transform.rotation * Vector3.up;
            unsafe{
                (*ptr).position.x = position.x;
                (*ptr).position.y = position.y;
                (*ptr).position.z = position.z;
                (*ptr).normal.x = normal.x;
                (*ptr).normal.y = normal.y;
                (*ptr).normal.z = normal.z;
                (*ptr).strength = strength; 
                (*ptr).minDist = minDistance; 
                (*ptr).maxDist = maxDistance; 
                (*ptr).falloff = falloff; 
            } 
        }
    }

    [CustomEditor(typeof(Vortex))]
    public class VortexEditor : Editor
    {
        public void OnSceneGUI()
        {
            var t = target as Vortex;


            EditorGUI.BeginChangeCheck();

            Handles.color = Color.red;
            float maxDistance = Handles.RadiusHandle(Quaternion.identity, t.transform.position, t.maxDistance);

            if (EditorGUI.EndChangeCheck()){
                Undo.RecordObject(target, "Changed Max Distance");
                t.maxDistance = maxDistance;
            }


            EditorGUI.BeginChangeCheck();
            
            Handles.color = Color.blue;
            float minDistance = Handles.RadiusHandle(Quaternion.identity, t.transform.position, t.minDistance);

            if (EditorGUI.EndChangeCheck()){
                Undo.RecordObject(target, "Changed Min Distance");
                t.minDistance = minDistance;
            }


            Handles.color = Color.white;
            Vector3 normal = t.transform.rotation * Vector3.up;
            for(int i = 0; i < 10; i++){
                if(t.falloff == Falloff.InvDist)
                    Handles.DrawWireDisc(t.transform.position, normal, t.strength*i*10/10);
                else if(t.falloff == Falloff.InvDist2)
                    Handles.DrawWireDisc(t.transform.position, normal, t.strength*i*i/10);
            }
        }
    }
}
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor; 

namespace Dotty {
    public class Attractor : MonoBehaviour {
        private int internalId; 
        private bool started; 
        unsafe private AttractorNtv* ptr; 
        
        public World world; 

        public float strength = 10; 
        public float minDistance = 0.1f; 
        public float maxDistance = 1000; 
        public Falloff falloff = Falloff.InvDist2; 

        void AddInternal() {
            Vector3 position = transform.position; 
            internalId = world.AddAttractor(position, strength, minDistance, maxDistance, falloff); 
            unsafe {
                ptr = world.GetAttractorPtr(internalId); 
            }
        }
        
        void RemoveInternal() {
            world.DestroyAttractor(internalId); 
        }

        void Start() {
            started = true; 
            AddInternal(); 
        }

        void OnEnable() {
            if(started) {
                AddInternal(); 
            }
        }

        void OnDisable() {
            RemoveInternal(); 
        }

        // Update is called once per frame
        void Update() {
            Vector3 position = transform.position; 
            unsafe{
                (*ptr).position.x = position.x;
                (*ptr).position.y = position.y;
                (*ptr).position.z = position.z;
                (*ptr).strength = strength; 
                (*ptr).minDist = minDistance; 
                (*ptr).maxDist = maxDistance; 
                (*ptr).falloff = falloff; 
            } 
        }
    }


    [CustomEditor(typeof(Attractor))]
    public class AttractorEditor : Editor {
        public void OnSceneGUI() {
            var t = target as Attractor;


            EditorGUI.BeginChangeCheck();

            Handles.color = Color.red;
            float maxDistance = Handles.RadiusHandle(Quaternion.identity, t.transform.position, t.maxDistance);

            if (EditorGUI.EndChangeCheck()) {
                Undo.RecordObject(target, "Changed Max Distance");
                t.maxDistance = maxDistance;
            }


            EditorGUI.BeginChangeCheck();
            
            Handles.color = Color.blue;
            float minDistance = Handles.RadiusHandle(Quaternion.identity, t.transform.position, t.minDistance);

            if (EditorGUI.EndChangeCheck()) {
                Undo.RecordObject(target, "Changed Min Distance");
                t.minDistance = minDistance;
            }

            Handles.color = Color.white;
            Vector3 normal = t.transform.rotation * Vector3.up;
            float s = t.strength*5; 

            Handles.DrawDottedLine(t.transform.position, t.transform.position + new Vector3(s, 0, 0), 4);
            Handles.DrawDottedLine(t.transform.position, t.transform.position + new Vector3(-s, 0, 0), 4);

            Handles.DrawDottedLine(t.transform.position, t.transform.position + new Vector3(0, s, 0), 4);
            Handles.DrawDottedLine(t.transform.position, t.transform.position + new Vector3(0, -s, 0), 4);

            Handles.DrawDottedLine(t.transform.position, t.transform.position + new Vector3(0, 0, s), 4);
            Handles.DrawDottedLine(t.transform.position, t.transform.position + new Vector3(0, 0, -s), 4);
        }
    }
}
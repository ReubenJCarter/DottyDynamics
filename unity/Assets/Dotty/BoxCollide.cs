using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

namespace Dotty{
    public class BoxCollide : MonoBehaviour
    {
        private int internalId; 
        unsafe private BoxColliderNtv* ptr; 
        
        public World world; 


        public Vector3 size; 
        public float staticFriction; 
        public float kineticFriction; 
        public bool inverse; 

        void OnEnable()
        {
            Vector3 position = transform.position;
            //internalId = world.AddBoxCollider(position, strength, minDistance, maxDistance, falloff); 
            unsafe{
                //ptr = world.GetBoxColliderPtr(internalId); 
            }
        }

        void OnDisable()
        {
            //world.DestroyBoxCollider(internalId); 
        }

        // Update is called once per frame
        void Update()
        {
            Vector3 position = transform.position; 
            unsafe{
                (*ptr).position.x = position.x;
                (*ptr).position.y = position.y;
                (*ptr).position.z = position.z; 
            } 
        }
    }

    [CustomEditor(typeof(BoxCollider))]
    public class BoxCollideEditor : Editor
    {
        public void OnSceneGUI()
        {
            var t = target as BoxCollider;



        }
    }
}
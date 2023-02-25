using System.Collections;
using System.Collections.Generic;
using UnityEngine;

#if UNITY_EDITOR
using UnityEditor;
#endif

namespace Dotty{
    public class BoxCollide : MonoBehaviour {
        private int internalId; 
        private bool started; 
        unsafe private BoxColliderNtv* ptr; 
        
        public World world; 

        public Mask layerMask = (Mask)0xFF; 

        public Vector3 size = new Vector3(0.5f, 0.5f, 0.5f); 
        public float staticFriction = 0; 
        public float kineticFriction = 0; 
        public bool inverse = false; 

        void AddInternal() {
            Vector3 position = transform.position; 
            Matrix4x4 invRot = Matrix4x4.Rotate(transform.rotation).inverse;
            internalId = world.AddBoxCollider(position, invRot, size, kineticFriction, staticFriction, inverse); 
            unsafe {
                ptr = world.GetBoxColliderPtr(internalId); 
            }
        }
        
        void RemoveInternal() {
            world.DestroyBoxCollider(internalId); 
        }

        // Start is called before the first frame update
        void Start() {
            if(world == null){
                world = World.instance; 
            }
            
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
        void Update()
        {
            Vector3 position = transform.position; 
            Matrix4x4 invRot = Matrix4x4.Rotate(transform.rotation).inverse;
            Mat3 ir = new Mat3(); 
            ir.x0 = invRot[0, 0]; 
            ir.y0 = invRot[1, 0]; 
            ir.z0 = invRot[2, 0]; 
            ir.x1 = invRot[0, 1]; 
            ir.y1 = invRot[1, 1]; 
            ir.z1 = invRot[2, 1]; 
            ir.x2 = invRot[0, 2]; 
            ir.y2 = invRot[1, 2]; 
            ir.z2 = invRot[2, 2]; 

            Vec3 sz = new Vec3(); 
            sz.x = size.x; 
            sz.y = size.y; 
            sz.z = size.z;

            unsafe{
                (*ptr).layerMask = (uint)layerMask; 
                (*ptr).position.x = position.x;
                (*ptr).position.y = position.y;
                (*ptr).position.z = position.z; 
                (*ptr).invRotation = ir; 
                (*ptr).size = sz; 
                (*ptr).staticFriction = staticFriction; 
                (*ptr).kineticFriction = kineticFriction;
                (*ptr).inverse = inverse;  
            } 
        }
    }

    #if UNITY_EDITOR
    [CustomEditor(typeof(BoxCollide))]
    public class BoxCollideEditor : Editor
    {
        private void DrawWireCube(BoxCollide t){
            Vector3 a = t.transform.TransformPoint( new Vector3(-t.size.x/2, -t.size.y/2, -t.size.z/2) ); 
            Vector3 b = t.transform.TransformPoint( new Vector3(-t.size.x/2, t.size.y/2, -t.size.z/2) ); 
            Vector3 c = t.transform.TransformPoint( new Vector3(t.size.x/2, t.size.y/2, -t.size.z/2) ); 
            Vector3 d = t.transform.TransformPoint( new Vector3(t.size.x/2, -t.size.y/2, -t.size.z/2) ); 

            Vector3 e = t.transform.TransformPoint( new Vector3(-t.size.x/2, -t.size.y/2, t.size.z/2) ); 
            Vector3 f = t.transform.TransformPoint( new Vector3(-t.size.x/2, t.size.y/2, t.size.z/2) ); 
            Vector3 g = t.transform.TransformPoint( new Vector3(t.size.x/2, t.size.y/2, t.size.z/2) ); 
            Vector3 h = t.transform.TransformPoint( new Vector3(t.size.x/2, -t.size.y/2, t.size.z/2) ); 

            Handles.DrawLine(a, b);
            Handles.DrawLine(b, c);
            Handles.DrawLine(c, d);
            Handles.DrawLine(d, a);

            Handles.DrawLine(e, f);
            Handles.DrawLine(f, g);
            Handles.DrawLine(g, h);
            Handles.DrawLine(h, e);

            Handles.DrawLine(a, e);
            Handles.DrawLine(b, f);
            Handles.DrawLine(c, g);
            Handles.DrawLine(d, h);
        }

        public void OnSceneGUI()
        {
            var t = target as BoxCollide;


            

            Handles.color = Color.white;
            //Handles.DrawWireCube(t.transform.position, t.size); 
            DrawWireCube(t); 

            float handleSz = 0.03f;

            //Handle x axis 
            EditorGUI.BeginChangeCheck();

            Vector3 targetPosition = t.transform.TransformPoint( new Vector3(t.size.x/2, 0f, 0f) ); 
            Vector3 targetAxis = t.transform.TransformDirection( new Vector3(1, 0, 0)); 
            Vector3 newTargetPosition = Handles.Slider(targetPosition, targetAxis, handleSz * HandleUtility.GetHandleSize(targetPosition), Handles.DotHandleCap, 0.1f);
            Vector3 newSize = t.transform.InverseTransformPoint( newTargetPosition ) * 2;

            if (EditorGUI.EndChangeCheck()) {
                Undo.RecordObject(target, "Changed Size");
                t.size.x = newSize.x; 
            }

            EditorGUI.BeginChangeCheck();
            
            targetPosition = t.transform.TransformPoint( new Vector3(-t.size.x/2, 0f, 0f) ); 
            targetAxis = t.transform.TransformDirection( new Vector3(-1, 0, 0)); 
            newTargetPosition = Handles.Slider(targetPosition, targetAxis, handleSz * HandleUtility.GetHandleSize(targetPosition), Handles.DotHandleCap, 0.1f);
            newSize = t.transform.InverseTransformPoint( newTargetPosition ) * -2;

            if (EditorGUI.EndChangeCheck()) {
                Undo.RecordObject(target, "Changed Size");
                t.size.x = newSize.x; 
            }

            //Handle y axis 
            EditorGUI.BeginChangeCheck();

            targetPosition = t.transform.TransformPoint( new Vector3(0, t.size.y/2, 0f) ); 
            targetAxis = t.transform.TransformDirection( new Vector3(0, 1, 0)); 
            newTargetPosition = Handles.Slider(targetPosition, targetAxis, handleSz * HandleUtility.GetHandleSize(targetPosition), Handles.DotHandleCap, 0.1f);
            newSize = t.transform.InverseTransformPoint( newTargetPosition ) * 2;

            if (EditorGUI.EndChangeCheck()) {
                Undo.RecordObject(target, "Changed Size");
                t.size.y = newSize.y; 
            }

            EditorGUI.BeginChangeCheck();
            
            targetPosition = t.transform.TransformPoint( new Vector3(0f, -t.size.y/2, 0f) ); 
            targetAxis = t.transform.TransformDirection( new Vector3(0, -1, 0)); 
            newTargetPosition = Handles.Slider(targetPosition, targetAxis, handleSz * HandleUtility.GetHandleSize(targetPosition), Handles.DotHandleCap, 0.1f);
            newSize = t.transform.InverseTransformPoint( newTargetPosition ) * -2;

            if (EditorGUI.EndChangeCheck()) {
                Undo.RecordObject(target, "Changed Size");
                t.size.y = newSize.y; 
            }

            //Handle z axis 
            EditorGUI.BeginChangeCheck();

            targetPosition = t.transform.TransformPoint( new Vector3(0, 0f, t.size.z/2) ); 
            targetAxis = t.transform.TransformDirection( new Vector3(0, 0, 1)); 
            newTargetPosition = Handles.Slider(targetPosition, targetAxis, handleSz * HandleUtility.GetHandleSize(targetPosition), Handles.DotHandleCap, 0.1f);
            newSize = t.transform.InverseTransformPoint( newTargetPosition ) * 2;

            if (EditorGUI.EndChangeCheck()) {
                Undo.RecordObject(target, "Changed Size");
                t.size.z = newSize.z; 
            }

            EditorGUI.BeginChangeCheck();
            
            targetPosition = t.transform.TransformPoint( new Vector3(0f, 0f, -t.size.z/2) ); 
            targetAxis = t.transform.TransformDirection( new Vector3(0, 0, -1)); 
            newTargetPosition = Handles.Slider(targetPosition, targetAxis, handleSz * HandleUtility.GetHandleSize(targetPosition), Handles.DotHandleCap, 0.1f);
            newSize = t.transform.InverseTransformPoint( newTargetPosition ) * -2;

            if (EditorGUI.EndChangeCheck()) {
                Undo.RecordObject(target, "Changed Size");
                t.size.z = newSize.z; 
            }

        }
    }
    #endif
}
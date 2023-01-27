using System.Collections;
using System.Collections.Generic;
using UnityEngine;

#if UNITY_EDITOR
using UnityEditor;
#endif

namespace Dotty{
    public class NoiseField : MonoBehaviour {
        private int internalId; 
        private bool started; 
        unsafe private NoiseFieldNtv* ptr; 
        
        public World world; 

        public float noiseScale = 10; 
        public NoiseType noiseType = NoiseType.SimplexCurl; 
        public float strength = 10; 
        public FieldMode mode = FieldMode.CorrectionForce; 

        public Vector3 boundSize = new Vector3(1, 1, 1); 
        public BoundShapeType boundShape = BoundShapeType.Infinite;
        public float boundThickness = 0; 
        public BoundFalloff boundFalloff = BoundFalloff.Linear;

        void AddInternal() {
            Vector3 position = transform.position; 
            Matrix4x4 invRot = Matrix4x4.Rotate(transform.rotation).inverse;
            internalId = world.AddNoiseField(position, noiseType, strength, noiseScale, mode, boundSize, boundShape, boundThickness, boundFalloff, invRot); 
            unsafe {
                ptr = world.GetNoiseFieldPtr(internalId); 
            }
        }
        
        void RemoveInternal() {
            world.DestroyNoiseField(internalId); 
        }

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
        void Update() {
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
            sz.x = boundSize.x; 
            sz.y = boundSize.y; 
            sz.z = boundSize.z;  

            unsafe{
                (*ptr).position.x = position.x;
                (*ptr).position.y = position.y;
                (*ptr).position.z = position.z; 
                (*ptr).noiseScale = noiseScale; 
                (*ptr).strength = strength;
                (*ptr).noiseType = noiseType;
                (*ptr).mode = mode;
                (*ptr).boundSize = sz; 
                (*ptr).boundShape = boundShape; 
                (*ptr).boundThickness = boundThickness; 
                (*ptr).boundFalloff = boundFalloff; 
                (*ptr).boundInvRotation = ir; 
            } 
        }
    }

    #if UNITY_EDITOR
    [CustomEditor(typeof(NoiseField))]
    public class NoiseFieldEditor : Editor {


        private void DrawWireCube(Transform tran, Vector3 boundSize){
            Vector3 a = tran.TransformPoint( new Vector3(-boundSize.x/2, -boundSize.y/2, -boundSize.z/2) ); 
            Vector3 b = tran.TransformPoint( new Vector3(-boundSize.x/2, boundSize.y/2, -boundSize.z/2) ); 
            Vector3 c = tran.TransformPoint( new Vector3(boundSize.x/2, boundSize.y/2, -boundSize.z/2) ); 
            Vector3 d = tran.TransformPoint( new Vector3(boundSize.x/2, -boundSize.y/2, -boundSize.z/2) ); 

            Vector3 e = tran.TransformPoint( new Vector3(-boundSize.x/2, -boundSize.y/2, boundSize.z/2) ); 
            Vector3 f = tran.TransformPoint( new Vector3(-boundSize.x/2, boundSize.y/2, boundSize.z/2) ); 
            Vector3 g = tran.TransformPoint( new Vector3(boundSize.x/2, boundSize.y/2, boundSize.z/2) ); 
            Vector3 h = tran.TransformPoint( new Vector3(boundSize.x/2, -boundSize.y/2, boundSize.z/2) ); 

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

        public void OnSceneGUI() {
            var t = target as NoiseField;

            if(t.boundShape == BoundShapeType.Sphere){



                EditorGUI.BeginChangeCheck();

                Handles.color = Color.white;
                float sz = Handles.RadiusHandle(Quaternion.identity, t.transform.position, t.boundSize.x);

                if (EditorGUI.EndChangeCheck()){
                    Undo.RecordObject(target, "Changed Size");
                    t.boundSize.x = sz;
                }


                EditorGUI.BeginChangeCheck();

                Handles.color = Color.red;
                float tkn = Handles.RadiusHandle(Quaternion.identity, t.transform.position, t.boundSize.x - t.boundThickness);

                if (EditorGUI.EndChangeCheck()){
                    Undo.RecordObject(target, "Changed Thickness");
                    t.boundThickness = t.boundSize.x - tkn;
                }



            }
            else if(t.boundShape == BoundShapeType.Box){

                Handles.color = Color.red;
                DrawWireCube(t.transform, t.boundSize - new Vector3(t.boundThickness, t.boundThickness, t.boundThickness) ); 

                Handles.color = Color.white;
                DrawWireCube(t.transform, t.boundSize); 

                float handleSz = 0.03f;

                //Handle x axis 
                EditorGUI.BeginChangeCheck();

                Vector3 targetPosition = t.transform.TransformPoint( new Vector3(t.boundSize.x/2, 0f, 0f) ); 
                Vector3 targetAxis = t.transform.TransformDirection( new Vector3(1, 0, 0)); 
                Vector3 newTargetPosition = Handles.Slider(targetPosition, targetAxis, handleSz * HandleUtility.GetHandleSize(targetPosition), Handles.DotHandleCap, 0.1f);
                Vector3 newSize = t.transform.InverseTransformPoint( newTargetPosition ) * 2;

                if (EditorGUI.EndChangeCheck()) {
                    Undo.RecordObject(target, "Changed Size");
                    t.boundSize.x = newSize.x; 
                }

                EditorGUI.BeginChangeCheck();
                
                targetPosition = t.transform.TransformPoint( new Vector3(-t.boundSize.x/2, 0f, 0f) ); 
                targetAxis = t.transform.TransformDirection( new Vector3(-1, 0, 0)); 
                newTargetPosition = Handles.Slider(targetPosition, targetAxis, handleSz * HandleUtility.GetHandleSize(targetPosition), Handles.DotHandleCap, 0.1f);
                newSize = t.transform.InverseTransformPoint( newTargetPosition ) * -2;

                if (EditorGUI.EndChangeCheck()) {
                    Undo.RecordObject(target, "Changed Size");
                    t.boundSize.x = newSize.x; 
                }

                //Handle y axis 
                EditorGUI.BeginChangeCheck();

                targetPosition = t.transform.TransformPoint( new Vector3(0, t.boundSize.y/2, 0f) ); 
                targetAxis = t.transform.TransformDirection( new Vector3(0, 1, 0)); 
                newTargetPosition = Handles.Slider(targetPosition, targetAxis, handleSz * HandleUtility.GetHandleSize(targetPosition), Handles.DotHandleCap, 0.1f);
                newSize = t.transform.InverseTransformPoint( newTargetPosition ) * 2;

                if (EditorGUI.EndChangeCheck()) {
                    Undo.RecordObject(target, "Changed Size");
                    t.boundSize.y = newSize.y; 
                }

                EditorGUI.BeginChangeCheck();
                
                targetPosition = t.transform.TransformPoint( new Vector3(0f, -t.boundSize.y/2, 0f) ); 
                targetAxis = t.transform.TransformDirection( new Vector3(0, -1, 0)); 
                newTargetPosition = Handles.Slider(targetPosition, targetAxis, handleSz * HandleUtility.GetHandleSize(targetPosition), Handles.DotHandleCap, 0.1f);
                newSize = t.transform.InverseTransformPoint( newTargetPosition ) * -2;

                if (EditorGUI.EndChangeCheck()) {
                    Undo.RecordObject(target, "Changed Size");
                    t.boundSize.y = newSize.y; 
                }

                //Handle z axis 
                EditorGUI.BeginChangeCheck();

                targetPosition = t.transform.TransformPoint( new Vector3(0, 0f, t.boundSize.z/2) ); 
                targetAxis = t.transform.TransformDirection( new Vector3(0, 0, 1)); 
                newTargetPosition = Handles.Slider(targetPosition, targetAxis, handleSz * HandleUtility.GetHandleSize(targetPosition), Handles.DotHandleCap, 0.1f);
                newSize = t.transform.InverseTransformPoint( newTargetPosition ) * 2;

                if (EditorGUI.EndChangeCheck()) {
                    Undo.RecordObject(target, "Changed Size");
                    t.boundSize.z = newSize.z; 
                }

                EditorGUI.BeginChangeCheck();
                
                targetPosition = t.transform.TransformPoint( new Vector3(0f, 0f, -t.boundSize.z/2) ); 
                targetAxis = t.transform.TransformDirection( new Vector3(0, 0, -1)); 
                newTargetPosition = Handles.Slider(targetPosition, targetAxis, handleSz * HandleUtility.GetHandleSize(targetPosition), Handles.DotHandleCap, 0.1f);
                newSize = t.transform.InverseTransformPoint( newTargetPosition ) * -2;

                if (EditorGUI.EndChangeCheck()) {
                    Undo.RecordObject(target, "Changed Size");
                    t.boundSize.z = newSize.z; 
                }
            }
        }
    }
    #endif
}
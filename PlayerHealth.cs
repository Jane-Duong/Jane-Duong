using UnityEngine;

public class PlayerHealth : MonoBehaviour
{
    [HideInInspector] public PlayerLives UIManager;  

    private void OnCollisionEnter2D(Collision2D collision)
    {
        if (collision.gameObject.CompareTag("Enemy") || collision.gameObject.CompareTag("EnemyBullet"))
        {
            

            if (UIManager != null)
            {
                Debug.Log("PlayerHealth: LoseLife called!");
                UIManager.LoseLife();
            }
            else
            {
                Debug.LogWarning("PlayerHealth: uiManager is NULL!");
            }
            Destroy(collision.gameObject);
            Destroy(gameObject);
          
        }
    }
    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.CompareTag("Enemy") || collision.CompareTag("EnemyBullet"))
        {
            if (UIManager != null)
            {
                Debug.Log("PlayerHealth: LoseLife (trigger)!"); 
                UIManager.LoseLife();
            }
            else
            {
                Debug.LogWarning("PlayerHealth: UIManager NULL (TRIGGER)!"); 
            }
            Destroy(collision.gameObject);
            Destroy(gameObject);
        }
    }
}

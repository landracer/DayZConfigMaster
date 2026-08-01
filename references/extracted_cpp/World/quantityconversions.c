// Source: F:/Games/Dayz/scripts/4_world/static/quantityconversions.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class QuantityConversions
{
    static string GetItemQuantityText( EntityAI item, bool showMax = false )
    {
        string quantity_text = &quot;&quot;;
        if ( item.IsInherited( InventoryItem) )
        {
            ItemBase item_base;
            Class.CastTo(item_base, item);
            float quantity = item_base.GetQuantity();
            int ammo;
            if ( item.IsMagazine() )
            {
                Magazine magazine_item;
                Class.CastTo(magazine_item, item);
                ammo = magazine_item.GetAmmoCount();
 
                return ammo.ToString();
            }
            else if ( item.IsInherited( ItemBook) )
            {
                return &quot;&quot;;
            }
            int stack_max = item.GetQuantityMax();
            
            //int max = item.ConfigGetInt(&quot;varQuantityMax&quot;);
            //string unit = item.ConfigGetString(&quot;stackedUnit&quot;);
 
            if (stack_max > 0)
            {
                if (stack_max == 1)
                {
                    if (quantity > 1)
                    {
                        if (showMax)
                            quantity_text = string.Format(&quot;%1/%2&quot;, quantity.ToString(), stack_max.ToString() );
                        //quantity_text = string.Format(&quot;%i/%i&quot;, quantity, stack_max );
                        else
                            quantity_text = quantity.ToString();
                    }
                    else
                    {
                        quantity_text = &quot;&quot;;
                    }
                }
                else
                {
                    if (showMax)
                        quantity_text = string.Format(&quot;%1/%2&quot;, quantity.ToString(), stack_max.ToString() );
                        //quantity_text = string.Format(&quot;%i/%i&quot;, quantity, stack_max );
                    else
                        quantity_text = quantity.ToString();
                    // if (unit == &quot;ml&quot;)
                    // {
                    //  float liters = round(quantity / 1000.0);
                    //  if ( quantity < 2000 )
                    //  {
                    //      liters =  ( round( (quantity / 100.0) ) ) / 10;
                    //  }
                    //  quantity_text = ftoa(liters) + &quot;l&quot;;
                    // }
                    // else
                    // {
                    //  quantity_text = itoa(quantity) + unit;
                    // }
                }
            }
        }
        return quantity_text;

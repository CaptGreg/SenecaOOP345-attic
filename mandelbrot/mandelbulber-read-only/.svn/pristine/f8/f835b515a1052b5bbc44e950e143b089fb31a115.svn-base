				zp = 2.0 * quaternionMul(z4, zp);
				newz = quaternionSqr(z4);
				z4 = newz + c4;
				r = length(z4);
				
				if (r < colourMin) colourMin = r;
				if(r>40.0f || any(isinf(z4))) 
				{
					float rp = length(zp);
					dist = 0.5f * r * native_log(r) / rp;
					out.colourIndex = colourMin * 5000.0f;
					break;
				}
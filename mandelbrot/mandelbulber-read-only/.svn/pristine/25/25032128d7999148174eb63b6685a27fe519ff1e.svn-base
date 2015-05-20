/*********************************************************
 /                   MANDELBULBER
 / functions for image post-processing
 /
 /
 / author: Krzysztof Marczak
 / contact: buddhi1980@gmail.com
 / licence: GNU GPL v3.0
 /
 ********************************************************/


#include <string.h>

#include "image.h"
#include "Render3D.h"
#include "interface.h"
#include "shaders.h"

sRGB *buddhabrotImg;
bool isBuddhabrot = false;
double buddhabrotAutoBright = 1.0;

guint64 histogram[256];
unsigned int histogram2[1000];

//******************* Nowa paleta kolorow ****************
void NewPalette(sRGB *p, double saturation)
{
	int R, G, B, Y;

	for (int i = 0; i < 255; i++)
	{
		Y = (Random(255) - 128) / (1.0 + saturation);
		p[i].R = R = Y + 128 + (Random(255) - 128) * saturation;
		p[i].G = G = Y + 128 + (Random(255) - 128) * saturation;
		p[i].B = B = Y + 128 + (Random(255) - 128) * saturation;
		if (R < 0) p[i].R = 0;
		if (G < 0) p[i].G = 0;
		if (B < 0) p[i].B = 0;
		if (R > 255) p[i].R = 255;
		if (G > 255) p[i].G = 255;
		if (B > 255) p[i].B = 255;
	}
	p[255].R = 255;
	p[255].G = 255;
	p[255].B = 255;
	p[254].R = 0;
	p[254].G = 0;
	p[254].B = 0;
	p[253].R = 0;
	p[253].G = 20;
	p[253].B = 40;

}

void PostRendering_DOF(cImage *image, double deep, double neutral)
{
	isPostRendering = true;

	int width = image->GetWidth();
	int height = image->GetHeight();

	sRGB16 *temp_image = new sRGB16[width * height];
	unsigned short *temp_alpha = new unsigned short[width * height];
	sSortZ<float> *temp_sort = new sSortZ<float>[width * height];
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int ptr = x + y * width;
			temp_image[ptr] = image->GetPixelImage16(x, y);
			temp_alpha[ptr] = image->GetPixelAlpha(x, y);
			temp_sort[ptr].z = image->GetPixelZBuffer(x, y);
			temp_sort[ptr].i = ptr;
		}
	}

	if (!noGUI && image->IsPreview())
	{
		gtk_progress_bar_set_text(GTK_PROGRESS_BAR(Interface.progressBar), "Rendering Depth Of Field effect. Sorting zBuffer");
		while (gtk_events_pending())
			gtk_main_iteration();
	}

	QuickSortZBuffer(temp_sort, 1, height * width - 1);

	if (!noGUI && image->IsPreview())
	{
		gtk_progress_bar_set_text(GTK_PROGRESS_BAR(Interface.progressBar), "Rendering Depth Of Field effect. Randomizing zBuffer");
		while (gtk_events_pending())
			gtk_main_iteration();
	}

	//Randomize Z-buffer
	int imgSize = height*width;
	for(int i=imgSize-1; i>=0; i--)
	{
		sSortZ<float> temp;
		temp = temp_sort[i];
		double z1 = temp.z;
		double size1 = (z1 - neutral) / z1 * deep;

		int randomStep = i;

		bool done = false;
		int ii;
		do
		{
			ii = i - Random(randomStep);
			if (ii <= 0) ii = 0;
			sSortZ<float> temp2 = temp_sort[ii];
			double z2 = temp2.z;
			double size2 = (z2 - neutral) / z2 * deep;

			if(size1 == 0 && size2 ==0) done = true;

			if (size1 * size2 > 0)
			{
				double sizeCompare;
				if (size1 > 0)
				{
					sizeCompare = size2 / size1;
				}
				else
				{
					sizeCompare = size1 / size2;
				}

				if (sizeCompare > 0.7)
				{
					done = true;
				}
				else
				{
					done = false;
				}
			}
			else
			{
				done = false;
			}
			randomStep = randomStep * 0.7 - 1;

			if(randomStep <= 0) done = true;
		}
		while(!done);
		temp_sort[i] = temp_sort[ii];
		temp_sort[ii] = temp;
	}

	if (!noGUI && image->IsPreview())
	{
		gtk_progress_bar_set_text(GTK_PROGRESS_BAR(Interface.progressBar), "Rendering Depth Of Field effect. Done 0%");
		while (gtk_events_pending())
			gtk_main_iteration();
	}

	double last_time = real_clock();
	int refreshCount = 0;

	for (int i = 0; i < height * width; i++)
	{
		int ii = temp_sort[height * width - i - 1].i;
		int x = ii % width;
		int y = ii / width;
		double z = image->GetPixelZBuffer(x, y);
		double blur = fabs((double) z - neutral) / z * deep;
		if (blur > 100) blur = 100.0;
		int size = blur;
		sRGB16 center = temp_image[x + y * width];
		unsigned short center_alpha = temp_alpha[x + y * width];
		double factor = blur * blur * sqrt(blur)* M_PI/3.0;

		for (int yy = y - size; yy <= y + size; yy++)
		{
			for (int xx = x - size; xx <= x + size; xx++)
			{
				if (xx >= 0 && xx < width && yy >= 0 && yy < height)
				{
					int dx = xx - x;
					int dy = yy - y;
					double r = sqrt((float)dx * dx + dy * dy);
					double op = (blur - r) / factor;
					if (op > 1.0) op = 1.0;
					if (op < 0.0) op = 0.0;
					if (op > 0.0)
					{
						double opN = 1.0 - op;
						sRGB16 old = image->GetPixelImage16(xx, yy);
						unsigned short old_alpha = image->GetPixelAlpha(xx, yy);
						sRGB16 pixel;
						pixel.R = old.R * opN + center.R * op;
						pixel.G = old.G * opN + center.G * op;
						pixel.B = old.B * opN + center.B * op;
						unsigned short alpha = old_alpha * opN + center_alpha * op;
						image->PutPixelImage16(xx, yy, pixel);
						image->PutPixelAlpha(xx, yy, alpha);
					}
				}
			}
		}
		double time = real_clock();
		if (time - last_time > 0.1 && !noGUI && image->IsPreview())
		{
			char progressText[1000];
			last_time = real_clock();
			double percent_done = (double) i / (height * width) * 100.0;
			sprintf(progressText, "Rendering Depth Of Field effect. Done %.2f%%", percent_done);
			gtk_progress_bar_set_text(GTK_PROGRESS_BAR(Interface.progressBar), progressText);

			if(refreshCount > 600)
			{
				//image->CompileImage();
				image->ConvertTo8bit();
				image->UpdatePreview();
				image->RedrawInWidget(renderWindow.drawingArea);
				refreshCount = 0;
			}
			refreshCount++;

			while (gtk_events_pending())
				gtk_main_iteration();
		}

		if (i % 1000 == 0)
		{
			double percentDone = (double)i/(width*height)*100.0;
			printf("Rendering Depth Of Field efect. Done %.2f%%       \r", percentDone);
			fflush(stdout);
		}
		if (!isPostRendering) break;
	}

	if (!noGUI && image->IsPreview())
	{
		gtk_progress_bar_set_text(GTK_PROGRESS_BAR(Interface.progressBar), "Rendering Depth Of Field effect. Done 100%");
		while (gtk_events_pending())
			gtk_main_iteration();
	}
	printf("Rendering Depth Of Field efect. Done 100%%       \n");

	isPostRendering = false;
	delete[] temp_image;
	delete[] temp_alpha;
	delete[] temp_sort;
}

void ThreadSSAO(void *ptr)
{

	sSSAOparams *param;
	param = (sSSAOparams*) ptr;

	int quality = param->quality;
	double persp = param->persp;
	int threadNo = param->threadNo;
	cImage *image = param->image;
	int width = image->GetWidth();
	int height = image->GetHeight();
	int progressive = param->progressive;
	bool quiet = param->quiet;

	double *cosinus = new double[quality];
	double *sinus = new double[quality];
	for (int i = 0; i < quality; i++)
	{
		sinus[i] = sin((double) i / quality * 2.0 * M_PI);
		cosinus[i] = cos((double) i / quality * 2.0 * M_PI);
	}

	double scale_factor = (double) width / (quality * quality) / 2.0;
	double aspectRatio = (double) width / height;

	enumPerspectiveType perspectiveType = param->perspectiveType;
	double fov = param->persp;

	sImageAdjustments *imageAdjustments = image->GetImageAdjustments();
	double intensity = imageAdjustments->globalIlum;

	for (int y = threadNo * progressive; y < height; y += NR_THREADS * progressive)
	{

		for (int x = 0; x < width; x += progressive)
		{
			double z = image->GetPixelZBuffer(x, y);
			unsigned short opacity16 = image->GetPixelOpacity(x, y);
			double opacity = opacity16 / 65535.0;
			double total_ambient = 0;

			if (z < 1e19)
			{
				//printf("SSAO point on object\n");
				double x2, y2;
				if (perspectiveType == fishEye)
				{
					x2 = M_PI * ((double) x / width - 0.5) * aspectRatio;
					y2 = M_PI * ((double) y / height - 0.5);
					double r = sqrt(x2 * x2 + y2 * y2);
					if(r != 0.0)
					{
						x2 = x2 / r * sin(r * fov) * z;
						y2 = y2 / r * sin(r * fov) * z;
					}
				}
				else if(perspectiveType == equirectangular)
				{
					x2 = M_PI * ((double) x / width - 0.5) * aspectRatio; //--------- do sprawdzenia
					y2 = M_PI * ((double) y / height - 0.5);
					x2 = sin(fov * x2) * cos(fov * y2) * z;
					y2 = sin(fov * y2) * z;
				}
				else
				{
					x2 = ((double) x / width - 0.5) * aspectRatio;
					y2 = ((double) y / height - 0.5);
					x2 = x2 * z * persp;
					y2 = y2 * z * persp;
				}

				double ambient = 0;

				for (int angle = 0; angle < quality; angle++)
				{
					double ca = cosinus[angle];
					double sa = sinus[angle];

					double max_diff = -1e50;

					for (double r = 1.0; r < quality; r += 1.0)
					{
						double rr = r * r * scale_factor;
						double xx = x + rr * ca;
						double yy = y + rr * sa;

						if ((int) xx == (int) x && (int) yy == (int) y) continue;
						if (xx < 0 || xx > width - 1 || yy < 0 || yy > height - 1) continue;
						double z2 = image->GetPixelZBuffer(xx, yy);

						double xx2, yy2;
						if (perspectiveType == fishEye)
						{
							xx2 = M_PI * ((double) xx / width - 0.5) * aspectRatio;
							yy2 = M_PI * ((double) yy / height - 0.5);
							double r2 = sqrt(xx2 * xx2 + yy2 * yy2);
							if(r != 0.0)
							{
								xx2 = xx2 / r2 * sin(r2 * fov) * z2;
								yy2 = yy2 / r2 * sin(r2 * fov) * z2;
							}
						}
						else if (perspectiveType == equirectangular)
						{
							xx2 = M_PI * (xx / width - 0.5) * aspectRatio; //--------- do sprawdzenia
							yy2 = M_PI * (yy / height - 0.5);
							xx2 = sin(fov * xx2) * cos(fov * yy2) * z2;
							yy2 = sin(fov * yy2) * z2;
						}
						else
						{
							xx2 = (xx / width - 0.5) * aspectRatio;
							yy2 = (yy / height - 0.5);
							xx2 = xx2 * (z2 * persp);
							yy2 = yy2 * (z2 * persp);
						}

						double dx = xx2 - x2;
						double dy = yy2 - y2;
						double dz = z2 - z;
						double dr = sqrt(dx * dx + dy * dy);
						double diff = -dz / dr;

						if (diff > max_diff) max_diff = diff;

					}
					double max_angle = atan(max_diff);

					ambient += -max_angle / M_PI + 0.5;

				}

				total_ambient = ambient / quality;
				if (total_ambient < 0) total_ambient = 0;

			}

			sRGB8 colour = image->GetPixelColor(x, y);
			sRGB16 pixel = image->GetPixelImage16(x, y);
			double R = pixel.R + 65535.0 * colour.R / 256.0 * total_ambient * intensity * (1.0 - opacity);
			double G = pixel.G + 65535.0 * colour.G / 256.0 * total_ambient * intensity * (1.0 - opacity);
			double B = pixel.B + 65535.0 * colour.B / 256.0 * total_ambient * intensity * (1.0 - opacity);
			if (R > 65535) R = 65535;
			if (G > 65535) G = 65535;
			if (B > 65535) B = 65535;
			for(int sqY =0; sqY<progressive; sqY++)
			{
				for(int sqX =0; sqX<progressive; sqX++)
				{
					pixel.R = R;
					pixel.G = G;
					pixel.B = B;
					image->PutPixelImage16(x + sqX, y + sqY, pixel);
				}
			}
		}

		double percentDone = (double) y / height * 100.0;
		if(!quiet) printf("Rendering Screen Space Ambient Occlusion. Done %.2f%%       \r", percentDone);
		fflush(stdout);

		param->done++;

		if (!isPostRendering) break;
	}
	delete[] sinus;
	delete[] cosinus;

}

void PostRendering_SSAO(cImage *image, double persp, int quality, enumPerspectiveType perspectiveType, bool quiet)
{
	isPostRendering = true;

	int height = image->GetHeight();

	if (!noGUI && image->IsPreview())
	{
		gtk_progress_bar_set_text(GTK_PROGRESS_BAR(Interface.progressBar), "Rendering Screen Space Ambient Occlusion");
		while (gtk_events_pending())
			gtk_main_iteration();
	}

	GThread **Thread = new GThread *[NR_THREADS];
	GError **err = new GError *[NR_THREADS];

	sSSAOparams *thread_param = new sSSAOparams[NR_THREADS];
	for (int i = 0; i < NR_THREADS; i++)
	{
		err[i] = NULL;
	}

	int progressive = image->progressiveFactor;

	//progressive = 1;

	for (int i = 0; i < NR_THREADS; i++)
	{
		//sending some parameters to thread
		thread_param[i].threadNo = i;
		thread_param[i].image = image;
		thread_param[i].persp = persp;
		thread_param[i].perspectiveType = perspectiveType;
		thread_param[i].quality = quality * sqrt(1.0 / progressive);
		thread_param[i].done = 0;
		thread_param[i].progressive = progressive;
		thread_param[i].quiet = quiet;

		//creating thread
		Thread[i] = g_thread_create((GThreadFunc) ThreadSSAO, &thread_param[i], TRUE, &err[i]);
	}

	double last_time = (double) clock() / CLOCKS_PER_SEC;

	int total_done;

	if (!noGUI)
	{
		do
		{
			total_done = 0;
			for (int i = 0; i < NR_THREADS; i++)
			{
				total_done += thread_param[i].done;
			}

			double time = (double) clock() / CLOCKS_PER_SEC;

			if (time - last_time > 0.5)
			{
				char progressText[1000];
				last_time = (double) clock() / CLOCKS_PER_SEC;
				double percent_done = (double) total_done / height * 100.0;
				sprintf(progressText, "Rendering Screen Space Ambient Occlusion. Done %.1f%%", percent_done);
				if(image->IsPreview())
				{
					gtk_progress_bar_set_text(GTK_PROGRESS_BAR(Interface.progressBar), progressText);
					while (gtk_events_pending())
						gtk_main_iteration();
				}
			}
		} while (total_done < height / progressive && isPostRendering);

		if(image->IsPreview())
		{
			gtk_progress_bar_set_text(GTK_PROGRESS_BAR(Interface.progressBar), "Rendering Screen Space Ambient Occlusion. Done 100%");
			while (gtk_events_pending())
				gtk_main_iteration();
		}
	}
	for (int i = 0; i < NR_THREADS; i++)
	{
		g_thread_join(Thread[i]);
		//printf("Rendering thread #%d finished\n", i + 1);
	}
	printf("Rendering Screen Space Ambient Occlusion. Done 100%%       \n");

	isPostRendering = false;

	delete[] Thread;
	delete[] err;
	delete[] thread_param;
}

void DrawHistogram(void)
{
	GdkGC *GC = gdk_gc_new(darea2->window);
	GdkColor color_black = { 0, 0, 0, 0 };
	gdk_gc_set_rgb_fg_color(GC, &color_black);
	gdk_draw_rectangle(darea2->window, GC, true, 0, 0, 256, 128);

	GdkColor color_red = { 0, 65535, 0, 0 };
	gdk_gc_set_rgb_fg_color(GC, &color_red);

	guint64 max = 0;
	for (int i = 0; i < 64; i++)
	{
		if (histogram[i] > max) max = histogram[i];
	}

	for (int i = 0; i < 64; i++)
	{
		int height = 127.0 * histogram[i] / max;
		gdk_draw_rectangle(darea2->window, GC, true, i * 4, 127 - height, 4, height);

	}
}

void DrawHistogram2(void)
{

	GdkGC *GC = gdk_gc_new(darea2->window);
	GdkColor color_black = { 0, 10000, 10000, 10000 };
	gdk_gc_set_rgb_fg_color(GC, &color_black);
	gdk_draw_rectangle(darea2->window, GC, true, 256, 0, 256, 128);
	GdkColor color_red = { 0, 0, 65535, 0 };
	gdk_gc_set_rgb_fg_color(GC, &color_red);

	unsigned int max = 0;
	for (int i = 0; i < 256; i++)
	{
		if (histogram2[i] > max) max = histogram2[i];
	}

	for (int i = 0; i < 256; i++)
	{
		int height = 127.0 * histogram2[i] / max;
		gdk_draw_rectangle(darea2->window, GC, true, i * 1 + 256, 127 - height, 1, height);

	}
}

void DrawPalette(sRGB *palette)
{
	memcpy(Interface_data.palette, palette, sizeof(sRGB)*256);

	if (paletteViewCreated)
	{
		double paletteOffset = gtk_adjustment_get_value(GTK_ADJUSTMENT(Interface.adjustmentPaletteOffset));
		double colWidth = 10;
		GdkGC *GC = gdk_gc_new(dareaPalette->window);
		for (int i = 0; i < 640; i++)
		{
			int number = (int) (i * 256.0 / colWidth + paletteOffset * 256.0);
			sRGB color = IndexToColour(number, palette);
			GdkColor gdk_color;
			gdk_color.red = color.R * 256;
			gdk_color.green = color.G * 256;
			gdk_color.blue = color.B * 256;
			gdk_color.pixel = 0;
			if(((int)(i + paletteOffset *10) % (int)colWidth) == colWidth / 2)
			{
				gdk_color.red = gdk_color.red > 32768 ? gdk_color.red - 5000 : gdk_color.red + 5000;
				gdk_color.green = gdk_color.green > 32768 ? gdk_color.green - 5000 : gdk_color.green + 5000;
				gdk_color.blue = gdk_color.blue > 32768 ? gdk_color.blue - 5000 : gdk_color.blue + 5000;
				gdk_color.pixel = 0;
			}
			gdk_gc_set_rgb_fg_color(GC, &gdk_color);
			gdk_draw_line(dareaPalette->window, GC, i, 0, i, 30);
		}
	}
}

void MakeStereoImage(cImage *left, cImage *right, guchar *stereoImage)
{
	int width = left->GetWidth();
	int height = left->GetHeight();

	sRGB8 *left8 = (sRGB8*) left->ConvertTo8bit();
	sRGB8 *right8 = (sRGB8*) right->ConvertTo8bit();

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			unsigned int addressSource = x + y * width;
			unsigned int addressDest = (x + y * width * 2) * 3;
			stereoImage[addressDest + 0] = right8[addressSource].R;
			stereoImage[addressDest + 1] = right8[addressSource].G;
			stereoImage[addressDest + 2] = right8[addressSource].B;
			stereoImage[addressDest + 0 + width * 3] = left8[addressSource].R;
			stereoImage[addressDest + 1 + width * 3] = left8[addressSource].G;
			stereoImage[addressDest + 2 + width * 3] = left8[addressSource].B;
		}
	}
}

void StereoPreview(cImage *temporaryImage, guchar *stereoImage)
{
	guchar *image8 = temporaryImage->ConvertTo8bit();
	int width = temporaryImage->GetWidth();
	int height = temporaryImage->GetHeight();
	memset(image8, 0, width * height * sizeof(sRGB8));

	for (int y = 0; y < height / 2; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int R = 0;
			int G = 0;
			int B = 0;
			unsigned int addressDest = (x + (y + height / 4) * width) * 3;
			for (int i = 0; i < 2; i++)
			{
				for (int j = 0; j < 2; j++)
				{
					unsigned int addressSource = ((x * 2 + i) + (y * 2 + j) * width * 2) * 3;
					R += stereoImage[addressSource + 0];
					G += stereoImage[addressSource + 1];
					B += stereoImage[addressSource + 2];
				}
			}
			R = R / 4;
			G = G / 4;
			B = B / 4;
			image8[addressDest + 0] = R;
			image8[addressDest + 1] = G;
			image8[addressDest + 2] = B;
		}
	}
	temporaryImage->UpdatePreview();
	temporaryImage->RedrawInWidget(renderWindow.drawingArea);
}


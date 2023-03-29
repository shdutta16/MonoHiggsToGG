from ROOT import *

def add_text(x1, x2, y1, y2, TEXT, color=1, alignment=22, angle = 0, argument="NDC"):
  import ROOT as r
  T = r.TPaveText(x1,y1,x2,y2, argument);
  T.SetFillColor(0);
  T.SetFillStyle(0);
  T.SetLineColor(0);
  T.SetTextAlign(alignment);
  T.SetTextColor(color);

  if (not isinstance(TEXT, str)):
     for this_text in TEXT:
        text = T.AddText(this_text);
        text.SetTextAngle(angle);
        text.SetTextAlign(alignment);
  else:
     text = T.AddText(TEXT);
     text.SetTextAngle(angle);
     text.SetTextAlign(alignment);
  T.SetTextFont(42);
  T.Draw("same");
  T.SetBorderSize(0);
  return T


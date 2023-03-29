from optparse import OptionParser, OptionGroup, make_option
import sys
import os

# -------------------------------------------------------------
class pyapp(object):

  def __init__(self,option_list,option_groups=[],defaults=None):
    self.objs_  = []
    self.canvs_ = []
    self.files_ = {} 

    # generic options 
    opt_list = [
      make_option("-I","--interactive",
                  action="store_true",dest="interactive",
                  default=sys.flags.interactive,help="default: %default",metavar=""),
      make_option("-O","--outdir",
                  action="store",dest="outdir",type="string",
                  default=None,help="default: %default"),
      make_option("-S","--save",
                  action="store_true",dest="save",
                  default=False,help="default: %default"),
      #make_option("--saveas",
      #            action="callback",dest="saveas",type="string",callback=ScratchAppen(),
      #            default=["png","root"], help="default: %default", metavar=""),
      make_option("-v","--verbose",
                  action="store_true",dest="verbose",
                  default=False,help="default: %default"),
      ] # end opt_list
 
    parser = OptionParser()

    opt_groups = [ ("PyApp Options", opt_list ) ] + option_groups
    if len(option_list) > 0:
      opt_groups.append( ("User Options", option_list) )

    for name,opts in opt_groups:
      gr = OptionGroup(parser,name)
      for opt in opts:
        gr.add_option(opt)
      parser.add_option_group(gr)

    (self.options, self.args) = parser.parse_args()

    # make output directory
    if self.options.outdir:
      if os.path.isdir(self.options.outdir):
        print("Will write to Output Directory: %s" %self.options.outdir)
      else: 
        print("Making Output Directory: %s" %self.options.outdir)
        os.mkdir(self.options.outdir)

    global ROOT
    import ROOT

  def run(self):
    self.__call__(self.options,self.args)

  def save(self,clear=False):
    for c in self.canvs_:
      if not c: continue
      c.Modified()
      for fmt in self.options.saveas:
        c.SaveAs("%s/%s.%s" %( self.options.outdir, c.GetName(), fmt ))
    if clear:
      for c in self.canvs_:
        del c
      self.canvs_ = []

#  def keep(self,objs,format=False):
#    if type(objs) == list:
#      for obj in objs:
#        self.keep(obj,format)
#      return
#    try:
#      if objs.IsA().InheritsFrom("TCanvas"):
#        self.canvs_.append(objs)
#      else:
#        self.objs_.append(objs)
#    except:
#      self.objs_.append(objs)
#    try:
#      if objs.IsA().InheritsFrom("TFile"):
#        key = "%s::%s" %( os.path.abspath(objs.GetName()), self.normalizeTFileOptions(objs.GetOption()))
#        self.files_[key] = objs
#    except:
#      pass
#    if format:
#      self.format(objs,self.options.styles)
#
#  def format(self,objs,styles):
#    for key,st in styles.iteritems():
#      if fnmatch(objs.GetName(),key) or objs.GetName() == key:
         

 

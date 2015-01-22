source analysisSettings.sh
source shell_functions.sh

missing_folder $main_directory
missing_folder $photon_comp
missing_folder $matchfalse
missing_folder $fits
missing_folder $delSeg_pt

photon_comp_pre=$(missing_folder_set_variable $photon_comp $pre)
matchfalse_pre=$(missing_folder_set_variable $matchfalse $pre)
fits_pre=$(missing_folder_set_variable $fits $pre)
delSeg_pt_pre=$(missing_folder_set_variable $delSeg_pt $pre)

photon_comp_post=$(missing_folder_set_variable $photon_comp $date)
matchfalse_post=$(missing_folder_set_variable $matchfalse $date)
fits_post=$(missing_folder_set_variable $fits $date)
delSeg_pt_post=$(missing_folder_set_variable $delSeg_pt $date)

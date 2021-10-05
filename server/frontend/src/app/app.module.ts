import { NgModule, CUSTOM_ELEMENTS_SCHEMA  } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';
import { FormsModule } from '@angular/forms';
import { HttpClientModule } from '@angular/common/http';
import { RouterModule } from '@angular/router';
import { ChartsModule } from 'ng2-charts';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { SweepComponent } from './sweep/sweep.component';
import { SweepService } from './service/sweep.service';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';
import { DeviceComponent } from './device/device.component';
import { DeviceService } from './service/device.service';
import { LocationComponent } from './location/location.component';
import { MapAllComponent } from './map-all/map-all.component';
import { StrengthChartComponent } from './strength-chart/strength-chart.component';
import { LoginComponent } from './login/login.component';
import { LoginService } from './service/login.service';
import { ReactiveFormsModule } from '@angular/forms';
import { HeaderComponent } from './header/header.component';
import { AuthService } from './service/auth.service';
import { BackButtonDisableModule } from 'angular-disable-browser-back-button';
import { DeviceInfoComponent } from './device-info/device-info.component';
import { DeviceTempComponent } from './device-temp/device-temp.component';
import { TrendService } from './service/trend.service';
import { TrendComponent } from './trend/trend.component';
import { ProjectsComponent } from './projects/projects.component';
import { MatButtonModule } from '@angular/material/button';
import { MatDialogModule } from '@angular/material/dialog';
import { LogoutmodalComponent } from './logoutmodal/logoutmodal.component';
import { RegistrationComponent } from './registration/registration.component';
import { RegistrationService } from './service/registration.service';
import { AlertsModule } from 'angular-alert-module';
import { ProjectsPageComponent } from './projects-page/projects-page.component';
import { ProjModalComponent } from './proj-modal/proj-modal.component';
import { ProjectDescModalComponent } from './project-desc-modal/project-desc-modal.component';



@NgModule({
  declarations: [
    AppComponent,
    SweepComponent,
    DeviceComponent,
    LocationComponent,
    MapAllComponent,
    StrengthChartComponent,
    LoginComponent,
    HeaderComponent,
    DeviceInfoComponent,
    DeviceTempComponent,
    TrendComponent,
    ProjectsComponent,
    LogoutmodalComponent,
    RegistrationComponent,
    ProjectsPageComponent,
    ProjModalComponent,
    ProjectDescModalComponent
  ],
  imports: [
    BrowserModule,
    FormsModule,
    AppRoutingModule,
    HttpClientModule,
    BrowserAnimationsModule,
    ChartsModule,
    ReactiveFormsModule,
    MatButtonModule,
    MatDialogModule,
    AlertsModule.forRoot(),
    BackButtonDisableModule.forRoot({
      preserveScrollPosition: true
    }),
    RouterModule.forRoot([
      {
        path: '',
        component: LoginComponent
      },
      {
        path: 'device/:deviceName',
        component: DeviceComponent
      },
      {
        path: 'location/:deviceName',
        component: LocationComponent
      },
      {
        path: 'map-devices',
        component: MapAllComponent
      },
      {
        path: 'sweep',
        component: SweepComponent
      },
      {
        path: 'device-info/:deviceName',
        component: DeviceInfoComponent
      },
      {
        path: 'device-temp/:deviceName',
        component: DeviceTempComponent
      },
      {
        path: 'projects',
        component: ProjectsComponent
      },
      {
        path: 'registration',
        component: RegistrationComponent
      },
      {
        path: 'proj',
        component: ProjectsPageComponent
      }
      
    ])
  ],
  providers: [
    SweepService,
    DeviceService,
    LoginService,
    AuthService,
    TrendService,
    RegistrationService,
    
  ],
  bootstrap: [AppComponent],
  schemas: [ CUSTOM_ELEMENTS_SCHEMA ]
})
export class AppModule { }
